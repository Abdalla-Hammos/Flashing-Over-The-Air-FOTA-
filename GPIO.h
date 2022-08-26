
#ifndef  GPIO_H_
#define  GPIO_H_

#include  <stm32f10x.h>
#include  "bitMainuplation.h"
#include  "STD_types.h"
#include  "RCC.h"



typedef enum
{
	GPIO_LOW = 0,
	GPIO_HIGH
}GPIO_PinLevel_t;



typedef enum
{
	GPIO_PORTA = 0,
	GPIO_PORTB = 1,
	GPIO_PORTC = 2
}GPIO_Port_t;



typedef enum
{
	GPIO_PIN0     = 0xFFFE,   // 1111 1111 1111 1110
	GPIO_PIN1     = 0xFFFD,   // 1111 1111 1111 1101
	GPIO_PIN2     = 0xFFFB,   // 1111 1111 1111 1011
	GPIO_PIN3     = 0xFFF7,   // 1111 1111 1111 0111
	GPIO_PIN4     = 0xFFEF,   // 1111 1111 1110 1111
	GPIO_PIN5     = 0xFFDF,   // 1111 1111 1101 1111
	GPIO_PIN6     = 0xFFBF,   // 1111 1111 1011 1111
	GPIO_PIN7     = 0xFF7F,   // 1111 1111 0111 1111
	GPIO_PIN8     = 0xFEFF,   // 1111 1110 1111 1111
	GPIO_PIN9     = 0xFDFF,   // 1111 1101 1111 1111
	GPIO_PIN10    = 0xFBFF,   // 1111 1011 1111 1111
	GPIO_PIN11    = 0xF7FF,   // 1111 0111 1111 1111
	GPIO_PIN12    = 0xEFFF,   // 1110 1111 1111 1111
	GPIO_PIN13    = 0xDFFF,   // 1101 1111 1111 1111
	GPIO_PIN14    = 0xBFFF,   // 1011 1111 1111 1111
	GPIO_PIN15    = 0x7FFF,   // 0111 1111 1111 1111
	GPIO_ALL_PINS = 0x0000    // 0000 0000 0000 0000
}GPIO_PinNumber_t;



typedef  enum
{
	INPUT_ANALOG,
	INPUT_FLOATING,
	INPUT_PULL_UP,
	INPUT_PULL_DOWN,
	GENERAL_PURPOSE_OUTPUT_OPEN_DRAIN,
	GENERAL_PURPOSE_OUTPUT_PUSH_PULL,
	ALTERNATE_FUNCTION_OUTPUT_PUSH_PULL,
	ALTERNATE_FUNCTION_OUTPUT_OPEN_DRAIN,
}GPIO_PinMode_t;



typedef  enum
{
	OUTPUT_SPEED_10MHz,
	OUTPUT_SPEED_2MHz,
	OUTPUT_SPEED_50MHz,
}GPIO_OutputSpeed_t;



typedef  struct
{
	GPIO_Port_t         port;
	GPIO_PinNumber_t    pinNumber;
}GPIO_Pin_t;



typedef  struct
{
	GPIO_PinMode_t      pinMode;
	GPIO_OutputSpeed_t  outputSpeed;
}GPIO_PinConfg_t;



void  GPIO_InitPin(const GPIO_Pin_t* groupOfPins, const GPIO_PinConfg_t* pinConfg);
void  GPIO_DeInitPin(const GPIO_Pin_t* groupOfPins);
void  GPIO_LockPinConfg(const GPIO_Pin_t* groupOfPins);
void  GPIO_SetPinOutputVoltage(const GPIO_Pin_t* groupOfPins, GPIO_PinLevel_t pinLevel);
GPIO_PinLevel_t  GPIO_ReadPinInputVoltage(const GPIO_Pin_t* pin);


#endif /* GPIO_H_ */
