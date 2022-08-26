#ifndef ESP_H_
#define ESP_H_

#include "STD_types.h"
#include "UART.h"



u8 ESP_ReadRxDataFromServer(u8** data, u8* dataLen);
void ESP_ClrBuffer(void);
void ESP_Init(void (*ptrToFun)(void));
u8 ESP_ConnectToRouter(u8* ssid , u8* password);
u8 ESP_SetWiFiMode(void);
u8 ESP_SetAsTCPClient(u8* serverIP , u8* portnum);
u8 ESP_EndTCPConnection(void);
u8 ESP_EnableUARTWiFiPassthroughMode(void);
u8 ESP_DisableUARTWiFiPassthroughMode(void);
u8 ESP_StartSendingData(u8* length,u8* data);



#endif
