
#ifndef INC_NVIC_H_
#define INC_NVIC_H_

#include  "stm32f10x.h"
#include  "STD_types.h"
#include  "bitMainuplation.h"

typedef enum
{
	IRQ_NOT_ACTIVE_NOT_PENDING    = 0b00,
	IRQ_ACTIVE                    = 0b01,
	IRQ_PENDING                   = 0b10,
	IRQ_ACTIVE_AND_PENDING        = 0b11
}IRQnState_t;




typedef enum
{
	FLASH_REGION    = 0,
	SRAM_REGION     = 1
}VectorTableRegion_t;



typedef enum
{
	PREEMTION_PERIOEITY_4_BIT  = 0x05FA0300,
	NO_SUB_PERIORITY           = PREEMTION_PERIOEITY_4_BIT,

	PREEMTION_PERIOEITY_3_BIT  = 0x05FA0400,
	SUB_PERIORITY_1_BIT        = PREEMTION_PERIOEITY_3_BIT,

	PREEMTION_PERIOEITY_2_BIT  = 0x05FA0500,
	SUB_PERIORITY_2_BIT        = PREEMTION_PERIOEITY_2_BIT,

	PREEMTION_PERIOEITY_1_BIT  = 0x05FA0600,
	SUB_PERIORITY_3_BIT        = PREEMTION_PERIOEITY_1_BIT,

	NO_PREEMTION_PERIOEITY     = 0x05FA0700,
	SUB_PERIORITY_4_BIT        = NO_PREEMTION_PERIOEITY,
}GroupPriorityLevel_t;




void  NVIC_EnableInterrupt (IRQn_Type IRQnID);
void  NVIC_DisableInterrupt(IRQn_Type IRQnID);
void  NVIC_SetInterruptPending(IRQn_Type IRQnID);
void  NVIC_ClrInterruptPending(IRQn_Type IRQnID);
IRQnState_t  NVIC_GetInterruptState(IRQn_Type IRQnID);
void  NVIC_RealocateVectorTable(VectorTableRegion_t region, u32 offsetAddress);
void  NVIC_ResetSystem(void);
void  NVIC_SetGroupPriorityLevel(GroupPriorityLevel_t  priorityLevel);
void  NVIC_SetInterruptPriority(IRQn_Type IRQnID, u8 preemptionPriority, u8 subPriority);




#endif /* INC_NVIC_H_ */
