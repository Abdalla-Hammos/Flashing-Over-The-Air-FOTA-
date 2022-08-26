#include "ESP.h"
#include "RCC.h"
#include "UART.h" 
#include "GPIO.h"

int main (void){
	
	GPIO_Pin_t groupOfPins = {
		.port = GPIO_PORTC,
		.pinNumber = GPIO_PIN13
	};
	GPIO_PinConfg_t pinConfg = {
		.pinMode = GENERAL_PURPOSE_OUTPUT_PUSH_PULL
	};
	
	GPIO_InitPin(&groupOfPins, &pinConfg);
	
	while(1)
	{
		GPIO_SetPinOutputVoltage(&groupOfPins, GPIO_HIGH);
		for(volatile int i=0; i<421366; i++);
		GPIO_SetPinOutputVoltage(&groupOfPins, GPIO_LOW);
		for(volatile int i=0; i<421366; i++);
	}
	
return 0;
}
