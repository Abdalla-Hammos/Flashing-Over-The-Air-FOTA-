
#ifndef INC_UART_H_
#define INC_UART_H_


#include "stm32f10x.h"
#include "STD_types.h"
#include "bitMainuplation.h"
#include "GPIO.h"
#include "RCC.h"
#include "NVIC.h"



#define  UART1     USART1
#define  UART2     USART2
#define  UART3     USART3






typedef enum
{
	NO_PARITY   = 1,
	EVEN_PARITY = 2,
	ODD_PARITY  = 3
}UART_Parity_t;



typedef enum
{
	ONE_STOP_BIT,
	HALF_STOP_BIT,
	TWO_STOP_BIT,
	ONE_AND_HALF_STOP_BIT
}UART_StopBits_t;



typedef enum
{
	UART_OVERRUN_ERROR,
	UART_NOISE_ERROR,
	UART_FRAMING_ERROR,
	UART_PARITY_ERROR,
	UART_NO_RX_DATA,
	UART_NO_ERROR
}UART_RxErrors_t;




typedef struct
{
	USART_TypeDef*   UARTx;                                          // Selection of which UART to be configured, it can be UART1 , UART2 or UART3

	STD_State_t      UART_TxState;                                   // Transmitter state, it can be ENABLE or DISABLE
	STD_State_t      UART_RxState;                                   // Receiver    state, it can be ENABLE or DISABLE
	STD_State_t      UART_TxCompleteInterruptState;                  // Transmission complete interrupt state, it can be ENABLE or DISABLE
	STD_State_t      UART_TxBufferEmptyInterruptState;               // Transmitter buffer empty interrupt state, it can be ENABLE or DISABLE
	STD_State_t      UART_RxInterruptState;                          // Receiver interrupt state, it can be ENABLE or DISABLE
	STD_State_t      UART_ParityErrorInterruptState;                 // Parity error interrupt state, it can be ENABLE or DISABLE
	u8               UART_DataBitsNum;                               // Number of data bits, it can be 8 or 9, if parity is used then the last data bit will carry parity bit
	UART_StopBits_t  UART_StopBitsNum;                               // Number of stop bits, it can be ONE_STOP_BIT , HALF_STOP_BIT , TWO_STOP_BIT or ONE_AND_HALF_STOP_BIT
	UART_Parity_t    UART_Parity;                                    // Parity selection, it can be EVEN_PARITY , ODD_PARITY or NO_PARITY
	u32              UART_BaudRate;                                  // Baud rate in bps (bit per second)

	void   (*UART_TxCompleteInterruptCallBackFunction)    (void);    // Call back function to be called when transmission complete interrupt occurs   , it's negligible if UART_TxCompleteInterruptState = DISABLE
	void   (*UART_TxBufferEmptyInterruptCallBackFunction) (void);    // Call back function to be called when Transmitter buffer empty interrupt occurs, it's negligible if UART_TxCompleteInterruptState = DISABLE
	void   (*UART_RxInterruptCallBackFunction)            (void);    // Call back function to be called when Receiver interrupt occurs                , it's negligible if UART_TxCompleteInterruptState = DISABLE
	void   (*UART_ParityErrorInterruptCallBackFunction)   (void);    // Call back function to be called when Parity Error interrupt occurs            , it's negligible if UART_TxCompleteInterruptState = DISABLE
}UART_Config_t;





void             UART_Init(UART_Config_t*  UART_Config);
void             UART_TxChar(USART_TypeDef* UARTx, u8 data);
UART_RxErrors_t  UART_RxCharBlocking(USART_TypeDef* UARTx, u8* data);
UART_RxErrors_t  UART_RxCharUnblocking(USART_TypeDef* UARTx, u8* data);
void             UART_TxString(USART_TypeDef* UARTx, u8* str);



#endif /* INC_UART_H_ */
