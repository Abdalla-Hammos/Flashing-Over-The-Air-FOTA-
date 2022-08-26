
#include  "NVIC.h"


void  NVIC_EnableInterrupt(IRQn_Type IRQnID)
{
	if(IRQnID >= 0)   //if IRQnID is an ID of Peripheral Interrupt not system exception
	{
		NVIC->ISER[IRQnID>>5] = 1<<(IRQnID & 0x1F);
	}
	else if(IRQnID == MemoryManagement_IRQn || IRQnID == BusFault_IRQn || IRQnID == UsageFault_IRQn)
	{
		SET_BIT(SCB->SHCSR, IRQnID + 28);
	}
}



void  NVIC_DisableInterrupt(IRQn_Type IRQnID)
{
	if(IRQnID >= 0)   //if IRQnID is an ID of Peripheral Interrupt not system exception
	{
		NVIC->ICER[IRQnID>>5] = 1<<(IRQnID & 0x1F);
	}
	else if(IRQnID == MemoryManagement_IRQn || IRQnID == BusFault_IRQn || IRQnID == UsageFault_IRQn)
	{
		CLR_BIT(SCB->SHCSR, IRQnID + 28);
	}
}



void  NVIC_SetInterruptPending(IRQn_Type IRQnID)
{
	if(IRQnID >= 0)   //if IRQnID is an ID of Peripheral Interrupt not system exception
	{
		NVIC->ISPR[IRQnID>>5] = 1<<(IRQnID & 0x1F);
	}
}



void  NVIC_ClrInterruptPending(IRQn_Type IRQnID)
{
	if(IRQnID >= 0)   //if IRQnID is an ID of Peripheral Interrupt not system exception
	{
		NVIC->ICPR[IRQnID>>5] = 1<<(IRQnID & 0x1F);
	}
}



IRQnState_t  NVIC_GetInterruptState(IRQn_Type IRQnID)
{
	if(IRQnID >= 0)   //if IRQnID is an ID of Peripheral Interrupt not system exception
	{
		u8 pendingBit = GET_BIT( NVIC->ISPR[IRQnID>>5] , IRQnID & 0x1F );
		u8 activeBit  = GET_BIT( NVIC->IABR[IRQnID>>5] , IRQnID & 0x1F );

		return  (activeBit | (pendingBit<<1));
	}
	return 10;
}




void  NVIC_RealocateVectorTable(VectorTableRegion_t region, u32 offsetAddress)
{
	if( (offsetAddress & 0x1FF) == 0 )   //offsetAddress must be aligned for 128 words (512 bit)
	{
		SCB->VTOR = (offsetAddress & 0x1FFFFE00) | (region<<29);
	}
}



void  NVIC_SetGroupPriorityLevel(GroupPriorityLevel_t  priorityLevel)
{
	SCB->AIRCR = priorityLevel;
}




void  NVIC_ResetSystem(void)
{
	SCB->AIRCR = 0x05FA0004;
}




void  NVIC_SetInterruptPriority(IRQn_Type IRQnID, u8 preemptionPriority, u8 subPriority)
{
	u8  groupPriorityLevelBits = (SCB->AIRCR & 0x700)>>8;
	u8  priority = subPriority | (preemptionPriority<<(groupPriorityLevelBits-3));

	if(IRQnID >= 0)
	{
		NVIC->IP[IRQnID] = priority<<4;
	}
	else if(IRQnID > -13)
	{
		SCB->SHP[IRQnID + 12] = priority<<4;
	}
}


