#include  "GPIO.h"



/********** Prototype of private functions **********/
static void  GPIO_InitOnePin(GPIO_Pin_t* pin, const GPIO_PinConfg_t* pinConfg);
static u32   GPIO_GetNewValueOfCR(GPIO_Pin_t* pin, const GPIO_PinConfg_t* pinConfg, u32 oldValueOfCR);



/**********       Private variables        **********/
static GPIO_TypeDef* GPIO[3] = {GPIOA, GPIOB, GPIOC};






/************ Functions implementation ************/






static void  GPIO_InitOnePin(GPIO_Pin_t* pin, const GPIO_PinConfg_t* pinConfg)
{
	if(pin->pinNumber < 8)
	{
		GPIO[pin->port]->CRL = GPIO_GetNewValueOfCR(pin, pinConfg, GPIO[pin->port]->CRL);
	}
	else if(pin->pinNumber < 16)
	{
		pin->pinNumber -= 8;
		GPIO[pin->port]->CRH = GPIO_GetNewValueOfCR(pin, pinConfg, GPIO[pin->port]->CRH);
		pin->pinNumber += 8;
	}

	switch(pinConfg->pinMode)
	{
		case INPUT_PULL_UP:
			GPIO[pin->port]->BSRR = 1<<(pin->pinNumber);
			break;
		case INPUT_PULL_DOWN:
			GPIO[pin->port]->BRR  = 1<<(pin->pinNumber);
			break;
		default:
			break;
	}
}





static u32   GPIO_GetNewValueOfCR(GPIO_Pin_t* pin, const GPIO_PinConfg_t* pinConfg, u32 oldValueOfCR)
{
	u32 CRValue = oldValueOfCR;

	/* Clear corresponding two MODE bits and two CNF bits */
	CRValue &= ~(0xFUL<<(4*(pin->pinNumber))) ;


	switch (pinConfg->pinMode)
	{
		case INPUT_ANALOG:
		case GENERAL_PURPOSE_OUTPUT_PUSH_PULL:
			/* Nothing to do here */
			break;

		case INPUT_FLOATING:
		case GENERAL_PURPOSE_OUTPUT_OPEN_DRAIN:
			SET_BIT(CRValue, 4*(pin->pinNumber)+2);
			break;

		case ALTERNATE_FUNCTION_OUTPUT_OPEN_DRAIN:
			SET_BIT(CRValue, 4*(pin->pinNumber)+2);

		case INPUT_PULL_UP:
		case INPUT_PULL_DOWN:
		case ALTERNATE_FUNCTION_OUTPUT_PUSH_PULL:
			SET_BIT(CRValue, 4*(pin->pinNumber)+3);
			break;

		default:
			break;
	}

	switch(pinConfg->pinMode)
	{
		case GENERAL_PURPOSE_OUTPUT_OPEN_DRAIN ... ALTERNATE_FUNCTION_OUTPUT_OPEN_DRAIN:
			switch (pinConfg->outputSpeed)
			{
				case OUTPUT_SPEED_10MHz:
					SET_BIT(CRValue, 4*(pin->pinNumber));
					break;

				case OUTPUT_SPEED_50MHz:
					SET_BIT(CRValue, 4*(pin->pinNumber));

				case OUTPUT_SPEED_2MHz:
					SET_BIT(CRValue, 4*(pin->pinNumber)+1);
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}

	return CRValue;
}





void  GPIO_InitPin(const GPIO_Pin_t* groupOfPins, const GPIO_PinConfg_t* pinConfg)
{
	GPIO_Pin_t pin;
	pin.port = groupOfPins->port;

	switch(pin.port)
	{
		case GPIO_PORTA:
			RCC_EnablePeripheralClock(GPIOA_PERIPHERAL);
			break;

		case GPIO_PORTB:
			RCC_EnablePeripheralClock(GPIOB_PERIPHERAL);
			break;

		case GPIO_PORTC:
			RCC_EnablePeripheralClock(GPIOC_PERIPHERAL);
			break;
	}


	for(u8 pinNumCounter=0; pinNumCounter<16; pinNumCounter++)
	{
		if(GET_BIT(groupOfPins->pinNumber, pinNumCounter) == 0)
		{
			pin.pinNumber = pinNumCounter;
			GPIO_InitOnePin(&pin, pinConfg);
		}
	}
}




void  GPIO_DeInitPin(const GPIO_Pin_t* groupOfPins)
{
	for(u8 pinNumCounter=0; pinNumCounter<8; pinNumCounter++)
	{
		if(GET_BIT(groupOfPins->pinNumber, pinNumCounter) == 0)
		{
			GPIO[groupOfPins->port]->CRL &= ~(0xFUL<<(4*pinNumCounter)) ;
			GPIO[groupOfPins->port]->CRL |= (0x4<<(4*pinNumCounter));
		}
	}

	for(u8 pinNumCounter=8; pinNumCounter<16; pinNumCounter++)
	{
		if(GET_BIT(groupOfPins->pinNumber, pinNumCounter) == 0)
		{
			GPIO[groupOfPins->port]->CRH &= ~(0xFUL<<(4*(pinNumCounter-8))) ;
			GPIO[groupOfPins->port]->CRH |= (0x4<<(4*(pinNumCounter-8)));
		}
	}

	GPIO[groupOfPins->port]->BRR = ~(groupOfPins->pinNumber);
}




void  GPIO_LockPinConfg(const GPIO_Pin_t* groupOfPins)
{
	u32 lockedPins = ( 0x0000FFFF & ~(groupOfPins->pinNumber) );
	GPIO[groupOfPins->port]->LCKR = lockedPins | 0x10000;
	GPIO[groupOfPins->port]->LCKR = lockedPins;
	GPIO[groupOfPins->port]->LCKR = lockedPins | 0x10000;
	GPIO[groupOfPins->port]->LCKR;
	GPIO[groupOfPins->port]->LCKR;
}





void  GPIO_SetPinOutputVoltage(const GPIO_Pin_t* groupOfPins, GPIO_PinLevel_t pinLevel)
{
	switch(pinLevel)
	{
		case GPIO_LOW:
			GPIO[groupOfPins->port]->BSRR = ~(groupOfPins->pinNumber);
			break;
		case GPIO_HIGH:
			GPIO[groupOfPins->port]->BRR  = ~(groupOfPins->pinNumber);
			break;
		default:
			break;
	}
}





GPIO_PinLevel_t  GPIO_ReadPinInputVoltage(const GPIO_Pin_t* pin)
{
	if( GPIO[pin->port]->IDR & (~(pin->pinNumber)) )
	{
		return GPIO_HIGH;
	}
	return GPIO_LOW;
}
