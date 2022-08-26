#include "RCC.h"

void  RCC_InitClock(void)
{
	RCC->CR = 0x1;   //enable HSI
	RCC->CFGR = 0;   //select HSI as system clock
	RCC->CIR  = 0x9F0000;   //disable all RCC interrupts , clear all flags
	
	RCC_EnablePeripheralClock(FLITF_PERIPHERAL);
	FPEC_InitFlash();

	/* check if configuration is valid or not */
	#if  HSI_STATE == DISABLED  &&  HSE_STATE == DISABLED  &&  PLL_STATE == DISABLED
		#error "wrong configuration, at least one clock must be ENABLED"
	#endif
	
	#if  HSI_STATE != ENABLED  &&  CLK_TYPE == HSI
		#error "wrong configuration, to use HSI as system clock it must be ENABLED"
	#endif
	
	#if  HSE_STATE != ENABLED  &&  CLK_TYPE == HSE
		#error "wrong configuration, to use HSE as system clock it must be ENABLED"
	#endif
	
	#if  PLL_STATE != ENABLED  &&  CLK_TYPE == PLL
		#error "wrong configuration, to use PLL as system clock it must be ENABLED"
	#endif
	
	#if  CLK_TYPE == PLL  &&  PLL_SRC == HSI  &&  HSI_STATE != ENABLED
		#error "wrong configuration, to use PLL as system clock with HSI as its input HSI must be ENABLED"
	#endif
	
	#if  CLK_TYPE == PLL  &&  PLL_SRC == HSE  &&  HSE_STATE != ENABLED
		#error "wrong configuration, to use PLL as system clock with HSE as its input HSE must be ENABLED"
	#endif
	
	#if  CLK_TYPE == PLL  &&  PLL_SRC != HSE  &&  PLL_SRC != HSI
		#error "wrong configuration, to use PLL as system clock its source must be HSI or HSE"
	#endif
	
	#if  CLK_TYPE == PLL  &&  (PLL_FACTOR < 2  ||  PLL_FACTOR > 16)
		#error "wrong configuration, PLL_FACTOR must be in range 2:16"
	#endif
	
	#if  HSE_VAL > HSE_MAX_CLK || HSE_VAL < HSE_MIN_CLK
		#error  "wrong HSE_VAL, it must be in range 4-16 MHz"
	#endif
	
	#if  SYSTEM_CLK > MAX_SYSTEM_CLK
		#error  "wrong configuration, system clock max is 72 MHz"
	#endif
	
	#if  APB1_CLK > APB1_MAX_CLK
		#error  "wrong configuration, APB1 clock max is 36 MHz"
	#endif    //end of checking configuration
	
	
	
	
	#if  AHB_DIVISION_FACTOR == 1
		/* do nothing, AHB division factor is already 1 */
	#elif  AHB_DIVISION_FACTOR == 2
		RCC->CFGR |= 0x80;
	#elif  AHB_DIVISION_FACTOR == 4
		RCC->CFGR |= 0x90;
	#elif  AHB_DIVISION_FACTOR == 8
		RCC->CFGR |= 0xA0;
	#elif  AHB_DIVISION_FACTOR == 16
		RCC->CFGR |= 0xB0;
	#elif  AHB_DIVISION_FACTOR == 64
		RCC->CFGR |= 0xC0;
	#elif  AHB_DIVISION_FACTOR == 128
		RCC->CFGR |= 0xD0;
	#elif  AHB_DIVISION_FACTOR == 256
		RCC->CFGR |= 0xE0;
	#elif  AHB_DIVISION_FACTOR == 512
		RCC->CFGR |= 0xF0;
	#else
		#error "wrong configuration, AHB_DIVISION_FACTOR must be one of the following --> 1 , 2 , 4 , 8 , 16 , 64 , 128 , 256 , 512"
	#endif
	
	
	
	
	#if  APB1_DIVISION_FACTOR == 1
		/* do nothing, AHB division factor is already 1 */
	#elif  APB1_DIVISION_FACTOR == 2
		RCC->CFGR |= 0x400;
	#elif  APB1_DIVISION_FACTOR == 4
		RCC->CFGR |= 0x500;
	#elif  APB1_DIVISION_FACTOR == 8
		RCC->CFGR |= 0x600;
	#elif  APB1_DIVISION_FACTOR == 16
		RCC->CFGR |= 0x700;
	#else
		#error "wrong configuration, APB1_DIVISION_FACTOR must be one of the following --> 1 , 2 , 4 , 8 , 16"
	#endif
	
	
	
	#if  APB2_DIVISION_FACTOR == 1
		/* do nothing, AHB division factor is already 1 */
	#elif  APB2_DIVISION_FACTOR == 2
		RCC->CFGR |= 0x2000;
	#elif  APB2_DIVISION_FACTOR == 4
		RCC->CFGR |= 0x2800;
	#elif  APB2_DIVISION_FACTOR == 8
		RCC->CFGR |= 0x3000;
	#elif  APB2_DIVISION_FACTOR == 16
		RCC->CFGR |= 0x3800;
	#else
		#error "wrong configuration, APB2_DIVISION_FACTOR must be one of the following --> 1 , 2 , 4 , 8 , 16"
	#endif
	
	

	#if  CLK_SECURITY_SYSTEM_STATE == ENABLED
		SET_BIT(RCC->CR, 19);
	#endif


	#if  HSE_STATE == ENABLED
		#if  HSE_SRC == HSE_RC
			SET_BIT(RCC->CR, 18);  //select RC as HSE
		#endif
		
		SET_BIT(RCC->CR, 16);   //enable HSE
		while(GET_BIT(RCC->CR, 17) == 0);   //wait until HSE is ready
		
		#if  CLK_TYPE == HSE
			SET_BIT(RCC->CFGR, 0);   //select HSE as system clock
			while( (RCC->CFGR & 0xC) != 0x4);   //wait until HSE is used as system clock
		#endif
	#endif
	
	
	
	#if  PLL_STATE == ENABLED
		#if  PLL_SRC == HSE
			SET_BIT(RCC->CFGR, 16);   //select HSE as PLL input clock 
		#elif  PLL_SRC == HSE_DIVIDED_BY_2
			SET_BIT(RCC->CFGR, 17);   //devide HSE by 2
			SET_BIT(RCC->CFGR, 16);   //select HSE/2 as PLL input clock 
		#elif  PLL_SRC == HSI_DIVIDED_BY_2
			/* do nothing, HSI/2 is already selected as PLL input clock */
		#endif
		
		RCC->CFGR |= ((PLL_FACTOR-2)<<18);   //select PLL multiplication factor
		SET_BIT(RCC->CR, 24);                //enable PLL
		while(GET_BIT(RCC->CR, 25) == 0);    //wait until PLL is ready
		
		#if  CLK_TYPE == PLL
			SET_BIT(RCC->CFGR, 1);   //select PLL as system clock
			while( (RCC->CFGR & 0xC) != 0x8);   //wait until PLL is used as system clock
		#endif
	#endif
	
	#if  HSI_STATE == DISABLED
		CLR_BIT(RCC->CR, 0);
	#endif
	
}


void  RCC_EnablePeripheralClock (uint8_t peripheral)
{
	if(peripheral < 32)    //if peripheral is connected to AHB
	{
		SET_BIT(RCC->AHBENR, peripheral);
	}
	else if(peripheral < 64)    //if peripheral is connected to APB1
	{
		SET_BIT(RCC->APB1ENR, peripheral-32);
	}
	else if(peripheral < 96)    //if peripheral is connected to APB2
	{
		SET_BIT(RCC->APB2ENR, peripheral-64);
	}
}

void  RCC_DisablePeripheralClock(uint8_t peripheral)
{
	if(peripheral < 32)    //if peripheral is connected to AHB
	{
		CLR_BIT(RCC->AHBENR, peripheral);
	}
	else if(peripheral < 64)    //if peripheral is connected to APB1
	{
		CLR_BIT(RCC->APB1ENR, peripheral-32);
	}
	else if(peripheral < 96)    //if peripheral is connected to APB2
	{
		CLR_BIT(RCC->APB2ENR, peripheral-64);
	}
}


void  RCC_PeripheralReset(uint8_t peripheral)
{
	if(peripheral >= 32)    //if peripheral is connected to APB1 or APB2
	{
		if(peripheral < 64)    //if peripheral is connected to APB1
		{
			RCC->APB1RSTR = 1<<(peripheral-32);
		}
		else if(peripheral < 96)    //if peripheral is connected to APB2
		{
			RCC->APB2RSTR = 1<<(peripheral-64);
		}
	}
}

