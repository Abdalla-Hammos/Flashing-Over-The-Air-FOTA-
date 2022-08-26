#include "UART.h"


/******************** Array of call back functions ***************************/
static void  (*(UART_TxCompleteInterruptCallBackFunction[3]))    (void)  =  {NULL, NULL, NULL};
static void  (*(UART_TxBufferEmptyInterruptCallBackFunction[3])) (void)  =  {NULL, NULL, NULL};
static void  (*(UART_RxInterruptCallBackFunction[3]))            (void)  =  {NULL, NULL, NULL};
static void  (*(UART_ParityErrorInterruptCallBackFunction[3]))   (void)  =  {NULL, NULL, NULL};


static GPIO_Pin_t  USART_TxPin[3] = {
	{
		.port       =  GPIO_PORTA,
		.pinNumber  =  GPIO_PIN9
	},
	{
		.port       =  GPIO_PORTA,
		.pinNumber  =  GPIO_PIN2
	},
	{
		.port       =  GPIO_PORTB,
		.pinNumber  =  GPIO_PIN10
	}
};

static GPIO_Pin_t  USART_RxPin[3] = {
	{
		.port       =  GPIO_PORTA,
		.pinNumber  =  GPIO_PIN10
	},
	{
		.port       =  GPIO_PORTA,
		.pinNumber  =  GPIO_PIN13
	},
	{
		.port       =  GPIO_PORTA,
		.pinNumber  =  GPIO_PIN11
	}
};

static GPIO_PinConfg_t  USART_TxPinConfig = {
	.pinMode      =  ALTERNATE_FUNCTION_OUTPUT_PUSH_PULL,
	.outputSpeed  =  OUTPUT_SPEED_2MHz
};

static GPIO_PinConfg_t  USART_RxPinConfig = {
	.pinMode      =  INPUT_FLOATING,
	.outputSpeed  =  OUTPUT_SPEED_2MHz
};





static UART_RxErrors_t  UART_CheckErrors(USART_TypeDef* UARTx);




static UART_RxErrors_t  UART_CheckErrors(USART_TypeDef* UARTx)
{
	if(GET_BIT(UARTx->SR, 3) == 1)	return UART_OVERRUN_ERROR;
	if(GET_BIT(UARTx->SR, 2) == 1)	return UART_NOISE_ERROR;
	if(GET_BIT(UARTx->SR, 1) == 1)	return UART_FRAMING_ERROR;
	if(GET_BIT(UARTx->SR, 0) == 1)	return UART_PARITY_ERROR;

	return UART_NO_ERROR;
}




void UART_Init(UART_Config_t*  UART_Config)
{
	u32 UART_Clk = 0;
	u8  UART_Idx = 0;
	switch((u32)UART_Config->UARTx)
	{
		case (u32)UART1:
			UART_Clk = USART1_CLK;
			UART_Idx = 0;
			RCC_EnablePeripheralClock(USART1_PERIPHERAL);
			break;

		case (u32)UART2:
			UART_Clk = USART2_CLK;
			UART_Idx = 1;
			RCC_EnablePeripheralClock(USART2_PERIPHERAL);
			break;

		case (u32)UART3:
			UART_Clk = USART3_CLK;
			UART_Idx = 2;
			RCC_EnablePeripheralClock(USART3_PERIPHERAL);
			break;
	}

	/* Set interrupt call back functions */
	UART_TxCompleteInterruptCallBackFunction[UART_Idx]    =   UART_Config->UART_TxCompleteInterruptCallBackFunction;
	UART_TxBufferEmptyInterruptCallBackFunction[UART_Idx] =   UART_Config->UART_TxBufferEmptyInterruptCallBackFunction;
	UART_RxInterruptCallBackFunction[UART_Idx]            =   UART_Config->UART_RxInterruptCallBackFunction;
	UART_ParityErrorInterruptCallBackFunction[UART_Idx]   =   UART_Config->UART_ParityErrorInterruptCallBackFunction;

	/* Configure UART pins */
	GPIO_InitPin(&USART_TxPin[UART_Idx], &USART_TxPinConfig);
	GPIO_InitPin(&USART_RxPin[UART_Idx], &USART_RxPinConfig);


	/* Configure baud rate */
	u16 USARTDIV_Mantissa = UART_Clk / (16 * UART_Config->UART_BaudRate);
	u16 USARTDIV_Fraction = ((u32)(UART_Clk / (f64)(UART_Config->UART_BaudRate) + 0.5)) % 16;

	UART_Config->UARTx->BRR = USARTDIV_Mantissa<<4 | USARTDIV_Fraction;

	UART_Config->UARTx->CR2 = (UART_Config->UART_StopBitsNum << 12);

	UART_Config->UARTx->CR1 = (1<<13) | ((UART_Config->UART_DataBitsNum - 8)<<12)
						| (UART_Config->UART_Parity << 9) | (UART_Config->UART_ParityErrorInterruptState << 8)
						| (UART_Config->UART_TxBufferEmptyInterruptState << 7)
						| (UART_Config->UART_TxCompleteInterruptState << 6)
						| (UART_Config->UART_RxInterruptState << 5)
						| (UART_Config->UART_TxState << 3) | (UART_Config->UART_RxState << 2);

	if(UART_Config->UART_RxInterruptState || UART_Config->UART_TxBufferEmptyInterruptState || UART_Config->UART_TxCompleteInterruptState || UART_Config->UART_ParityErrorInterruptState)
	{
		NVIC_EnableInterrupt(USART1_IRQn + UART_Idx);
	}

}






void  UART_TxChar(USART_TypeDef* UARTx, u8 data)
{
	/* If bit 7 = 0 --> Transmit data register is NOT empty --> do nothing */
	while(GET_BIT(UARTx->SR, 7) == 0);

	UARTx->DR = data;  //Put data in Transmit data register
}


UART_RxErrors_t  UART_RxCharBlocking(USART_TypeDef* UARTx, u8* data)
{
	/* If bit 5 = 0 --> Data is NOT received --> do nothing */
	while(GET_BIT(UARTx->SR, 5) == 0);

	*data = UARTx->DR;  //Read data

	return UART_CheckErrors(UARTx);
}


UART_RxErrors_t  UART_RxCharUnblocking(USART_TypeDef* UARTx, u8* data)
{
	/* If bit 5 = 0 --> Data is NOT received --> return UART_NO_RX_DATA */
	if(GET_BIT(UARTx->SR, 5) == 0)		return UART_NO_RX_DATA;

	*data = UARTx->DR;  //Read data

	return UART_CheckErrors(UARTx);
}


void  UART_TxString(USART_TypeDef* UARTx, u8* str)
{
	while(*str != 0)
	{
		UART_TxChar(UARTx, *str);
		str++;
	}
}








void USART1_IRQHandler(void)
{
	if(GET_BIT(UART1->SR, 7) == 1  &&  UART_TxBufferEmptyInterruptCallBackFunction[0] != NULL)
	{
		(UART_TxBufferEmptyInterruptCallBackFunction[0])();
	}
	else if(GET_BIT(UART1->SR, 6) == 1  &&  UART_TxCompleteInterruptCallBackFunction[0] != NULL)
	{
		(UART_TxCompleteInterruptCallBackFunction[0])();
	}
	else if(GET_BIT(UART1->SR, 5) == 1  &&  UART_RxInterruptCallBackFunction[0] != NULL)
	{
		(UART_RxInterruptCallBackFunction[0])();
	}
	else if(GET_BIT(UART1->SR, 0) == 1  &&  UART_ParityErrorInterruptCallBackFunction[0] != NULL)
	{
		(UART_ParityErrorInterruptCallBackFunction[0])();
	}
}





void USART2_IRQHandler(void)
{
	if(GET_BIT(UART2->SR, 7) == 1  &&  UART_TxBufferEmptyInterruptCallBackFunction[1] != NULL)
	{
		(UART_TxBufferEmptyInterruptCallBackFunction[1])();
	}
	else if(GET_BIT(UART2->SR, 6) == 1  &&  UART_TxCompleteInterruptCallBackFunction[1] != NULL)
	{
		(UART_TxCompleteInterruptCallBackFunction[1])();
	}
	else if(GET_BIT(UART2->SR, 5) == 1  &&  UART_RxInterruptCallBackFunction[1] != NULL)
	{
		(UART_RxInterruptCallBackFunction[1])();
	}
	else if(GET_BIT(UART2->SR, 0) == 1  &&  UART_ParityErrorInterruptCallBackFunction[1] != NULL)
	{
		(UART_ParityErrorInterruptCallBackFunction[1])();
	}
}




void USART3_IRQHandler(void)
{
	if(GET_BIT(UART3->SR, 7) == 1  &&  UART_TxBufferEmptyInterruptCallBackFunction[2] != NULL)
	{
		(UART_TxBufferEmptyInterruptCallBackFunction[2])();
	}
	else if(GET_BIT(UART3->SR, 6) == 1  &&  UART_TxCompleteInterruptCallBackFunction[2] != NULL)
	{
		(UART_TxCompleteInterruptCallBackFunction[2])();
	}
	else if(GET_BIT(UART3->SR, 5) == 1  &&  UART_RxInterruptCallBackFunction[2] != NULL)
	{
		(UART_RxInterruptCallBackFunction[2])();
	}
	else if(GET_BIT(UART3->SR, 0) == 1  &&  UART_ParityErrorInterruptCallBackFunction[2] != NULL)
	{
		(UART_ParityErrorInterruptCallBackFunction[2])();
	}
}

