
#ifndef INC_FPEC_H_
#define INC_FPEC_H_

#include <stm32f10x.h>
#include "bitMainuplation.h"
#include "STD_types.h"
#include "RCC.h"



#define  FLASH_BASE_ADDRESS      (0x08000000UL)
#define  WORD_SIZE               4
#define  HALF_WORD_SIZE          2
#define  PAGE_SIZE_BYTES         1024
#define  PAGE_SIZE_HALF_WORDS    512
#define  PAGE_SIZE_WORDS         256


/* Errors that can occur while programming flash */
typedef enum
{
	FPEC_PROGRAMMING_ERROR,       //Error due to programming flash in a wrong way
	FPEC_WRITE_PROTECTION_ERROR,  //Error due to programming protected location
	FPEC_NO_ERRORS
}FPEC_Error_t;


void          FPEC_InitFlash(void);
FPEC_Error_t  FPEC_ErasePage(u8 pageNumber);
FPEC_Error_t  FPEC_WritePage(u8 pageNumber, u16* data);
void          FPEC_ReadPage (u8 pageNumber, u16* data);
FPEC_Error_t  FPEC_DirectWriteHalfWord(u8 pageNumber, u16 offsetAddress, u16 data);
FPEC_Error_t  FPEC_DirectWriteByte(u8 pageNumber, u16 offsetAddress, u8 data);


FPEC_Error_t  FPEC_WriteHalfWord(u8 pageNumber, u16 offsetAddress, u16  data);
u16           FPEC_ReadHalfWord (u8 pageNumber, u16 offsetAddress);
FPEC_Error_t  FPEC_WriteByte(u8 pageNumber, u16 offsetAddress, u8 data);
u8            FPEC_ReadByte (u8 pageNumber, u16 offsetAddress);
FPEC_Error_t  FPEC_WriteWord(u8 pageNumber, u8  offsetAddress, u32  data);
u32           FPEC_ReadWord (u8 pageNumber, u8  offsetAddress);



#endif /* INC_FPEC_H_ */
