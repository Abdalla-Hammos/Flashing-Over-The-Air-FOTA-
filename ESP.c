#include  "ESP.h"


static u8 ESP_RxData[100];
static u8 ESP_RxDataSize = 0;


static void (*callBackFun)(void);


/* when nnn characters (xxxxxxxxxx) are received from server, ESP_RxData should contain --> +IPD,nnn:xxxxxxxxxx */
#define  SERVER_DATA_MIN_HEADER_SIZE       7
#define  SERVER_DATA_LENGTH_STRT_IDX       5


static Boolean_t isStringStartsWith(u8* str1, u8* str2)
{
	while(*str1 != '\0' && *str2 != 0)
	{
		if(*str1 != *str2)
		{
			return FALSE;
		}
		str1++;
		str2++;
	}
	if(*str2 != 0) return FALSE;
	return TRUE;
}




static u8 ESP_CheckErrors(u8* okresp, u8* notokresp)
{
	u8 oklength =0;
	u8 notoklength =0;
	while(okresp[oklength]){
		oklength++;
	}
	while(notokresp[notoklength]){
		notoklength++;
	}

	while(TRUE)
	{
		if(ESP_RxDataSize >= oklength  &&  isStringStartsWith(&ESP_RxData[ESP_RxDataSize - oklength], okresp))
		{
			return OK;
		}
		else if(ESP_RxDataSize >= notoklength  &&  isStringStartsWith(&ESP_RxData[ESP_RxDataSize - notoklength], notokresp))
		{
			return NOT_OK;
		}
	}
}


static void ESP_RxCallBackFun(void)
{
	UART_RxCharUnblocking(UART1, &ESP_RxData[ESP_RxDataSize]);
	ESP_RxDataSize++;

	/* when nnn characters (xxxxxxxxxx) are received from server, ESP_RxData should contain --> +IPD,nnn:xxxxxxxxxx */
	if(ESP_RxDataSize > SERVER_DATA_MIN_HEADER_SIZE  &&  isStringStartsWith(ESP_RxData, (u8*)"+IPD,"))
	{
		u8 dataFromServerSize = 0;
		u8 i = SERVER_DATA_LENGTH_STRT_IDX;
		while((i < ESP_RxDataSize) && (ESP_RxData[i] != ':'))
		{
			dataFromServerSize *= 10;
			dataFromServerSize += ESP_RxData[i] - '0';
			i++;
		}

		if(dataFromServerSize == ESP_RxDataSize - (i + 1))
		{
			callBackFun();
		}
	}
}

u8 ESP_ReadRxDataFromServer(u8** data, u8* dataLen)
{
	if(ESP_RxDataSize > SERVER_DATA_MIN_HEADER_SIZE  &&  isStringStartsWith(ESP_RxData, (u8*)"+IPD,"))
	{
		*dataLen = 0;
		u8 i = SERVER_DATA_LENGTH_STRT_IDX;
		while((i < ESP_RxDataSize) && (ESP_RxData[i] != ':'))
		{
			*dataLen *= 10;
			*dataLen += ESP_RxData[i] - '0';
			i++;
		}

		*data = &ESP_RxData[i + 1];
		if(*dataLen == ESP_RxDataSize - (i + 1))
		{
			return OK;
		}
	}
	return NOT_OK;
}


void ESP_ClrBuffer(void)
{
	ESP_RxDataSize = 0;
}

void ESP_Init(void (*ptrToFun)(void))
{
	callBackFun = ptrToFun;

	UART_Config_t UART_Config = {
		.UARTx                                            =   UART1,

		.UART_TxState                                     =   ENABLE,
		.UART_RxState                                     =   ENABLE,
		.UART_TxCompleteInterruptState                    =   DISABLE,
		.UART_TxBufferEmptyInterruptState                 =   DISABLE,
		.UART_RxInterruptState                            =   ENABLE,
		.UART_ParityErrorInterruptState                   =   DISABLE,
		.UART_DataBitsNum                                 =   8,
		.UART_StopBitsNum                                 =   ONE_STOP_BIT,
		.UART_Parity                                      =   NO_PARITY,
		.UART_BaudRate                                    =   115200,

		.UART_TxCompleteInterruptCallBackFunction         =   NULL,
		.UART_TxBufferEmptyInterruptCallBackFunction      =   NULL,
		.UART_RxInterruptCallBackFunction                 =   ESP_RxCallBackFun,
		.UART_ParityErrorInterruptCallBackFunction        =   NULL
	};

	UART_Init(&UART_Config);
}


u8 ESP_ConnectToRouter(u8* ssid, u8* password)
{
	UART_TxString(UART1, (u8*)"AT+CWJAP=\"");
	UART_TxString(UART1, ssid);
	UART_TxString(UART1, (u8*)"\",\"");
	UART_TxString(UART1, password);
	UART_TxString(UART1, (u8*)"\"\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	ESP_RxDataSize = 0;
	return status;
}


u8 ESP_SetWiFiMode(void)
{
	UART_TxString(UART1, (u8*)"AT+CWMODE=3\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	ESP_RxDataSize = 0;
	return status;
}


u8 ESP_SetAsTCPClient(u8* serverIP, u8* portnum)
{
	UART_TxString(UART1, (u8*)"AT+CIPSTART=\"TCP\",\"");
	UART_TxString(UART1, serverIP);
	UART_TxString(UART1, (u8*)"\",");
	UART_TxString(UART1, portnum);
	UART_TxString(UART1,  (u8*)"\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	ESP_RxDataSize = 0;
	return status;

}


u8 ESP_EndTCPConnection(void)
{
	UART_TxString(UART1, (u8*)"AT+CIPCLOSE\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	ESP_RxDataSize = 0;
	return status;
	 
}


u8 ESP_EnableUARTWiFiPassthroughMode(void)
{
	UART_TxString(UART1, (u8*)"AT+CIPMODE=1\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	ESP_RxDataSize = 0;
	return status;
}


u8 ESP_DisableUARTWiFiPassthroughMode(void)
{
	UART_TxString(UART1, (u8*)"AT+CIPMODE=0\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	ESP_RxDataSize = 0;
	return status;
}


u8 ESP_StartSendingData(u8* length,u8* data)
{
	UART_TxString(UART1, (u8*)"AT+CIPSEND=");
	UART_TxString(UART1, length);
	UART_TxString(UART1,  (u8*)"\r\n");
	u8 status = ESP_CheckErrors((u8*)"OK\r\n",(u8*)"ERROR\r\n");
	if(status==OK){
		while(ESP_RxData[ESP_RxDataSize-1]!='>');
	    ESP_ClrBuffer();
		UART_TxString(UART1, data);
		status= ESP_CheckErrors((u8*)"OK\r\n",(u8*)"FAIL\r\n");
		ESP_ClrBuffer();
		return status;
	}else{
		ESP_ClrBuffer();
		return NOT_OK;
	}
	
}


