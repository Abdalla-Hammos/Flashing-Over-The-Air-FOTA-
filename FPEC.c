#include "FPEC.h"


#define  KEY1                    (0x45670123UL)
#define  KEY2                    (0xCDEF89ABUL)




/********** Prototype of private functions **********/
static void          FPEC_UnlockFlash(void);
static void          FPEC_LockFlash(void);
static FPEC_Error_t  FPEC_CheckErrors(void);

/************ Functions implementation ************/



/**
  * @brief  Unlock the Embedded Flash.
  * @param  None.
  * @retval None.
  */
static void  FPEC_UnlockFlash(void)
{
	FLASH->KEYR = KEY1;  //write KEY1
	FLASH->KEYR = KEY2;  //write KEY2
}




/**
  * @brief  Lock the Embedded Flash.
  * @param  None.
  * @retval None.
  */
static void  FPEC_LockFlash(void)
{
	SET_BIT(FLASH->CR, 7);
}




/**
  * @brief  Check if any errors occurred while programming/erasing flash
  * @param  None
  * @retval Flash error
  */
static FPEC_Error_t  FPEC_CheckErrors(void)
{
	if(GET_BIT(FLASH->SR, 4) == 1)
	{
		SET_BIT(FLASH->SR, 4);  //clear flag
		return  FPEC_WRITE_PROTECTION_ERROR;
	}
	if(GET_BIT(FLASH->SR, 2) == 1)
	{
		SET_BIT(FLASH->SR, 2);  //clear flag
		return  FPEC_PROGRAMMING_ERROR;
	}
	return FPEC_NO_ERRORS;
}



/**
  * @brief  Write half word (2 bytes) directly in the flash memory.
  * @note   Before using this function, the location you try to write on must be erased first (contains 0xFFFF).
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page where you want to write.
  * @param  offsetAddress: offset address of the half word you want to write.
  * @note   offsetAddress must be even number
  * @param  data: actual data you want to write.
  * @retval Flash error
  */
FPEC_Error_t  FPEC_DirectWriteHalfWord(u8 pageNumber, u16 offsetAddress, u16 data)
{
	if(offsetAddress % 2 == 1)	return FPEC_PROGRAMMING_ERROR;

	FPEC_Error_t writeStatus = FPEC_NO_ERRORS;

	FPEC_UnlockFlash();

	/* Wait if the flash is busy */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	SET_BIT(FLASH->CR, 0);   //Set bit 0 to select flash programming

	/* Get physical address of the desired half word */
	volatile u16* physicalAddress = (volatile u16*)(FLASH_BASE_ADDRESS + pageNumber * PAGE_SIZE_BYTES + offsetAddress);
	*physicalAddress = data;

	/* Wait until writing is completed */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	CLR_BIT(FLASH->CR, 0);   //clear bit 0 to unselect flash programming

	/* Read the written half word to make sure it's written successfully */
	if(FPEC_ReadHalfWord(pageNumber, offsetAddress) == data)
	{
		writeStatus = FPEC_NO_ERRORS;
	}
	else
	{
		writeStatus =  FPEC_CheckErrors();
	}

	FPEC_LockFlash();

	return writeStatus;
}





/**
  * @brief  Write 1 byte directly in the flash memory.
  * @note   Before using this function, the location you try to write on must be erased first (contains 0xFF).
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page where you want to write.
  * @param  offsetAddress: offset address of the half word you want to write.
  * @param  data: actual data you want to write.
  * @retval Flash error
  */
FPEC_Error_t  FPEC_DirectWriteByte(u8 pageNumber, u16 offsetAddress, u8 data)
{
	FPEC_Error_t writeStatus = FPEC_NO_ERRORS;

	FPEC_UnlockFlash();

	/* Wait if the flash is busy */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	SET_BIT(FLASH->CR, 0);   //Set bit 0 to select flash programming

	/* Get physical address of the desired half word */
	volatile u8* physicalAddress = (volatile u8*)(FLASH_BASE_ADDRESS + pageNumber * PAGE_SIZE_BYTES + offsetAddress);
	*physicalAddress = data;

	/* Wait until writing is completed */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	CLR_BIT(FLASH->CR, 0);   //clear bit 0 to unselect flash programming

	/* Read the written half word to make sure it's written successfully */
	if(FPEC_ReadByte(pageNumber, offsetAddress) == data)
	{
		writeStatus = FPEC_NO_ERRORS;
	}
	else
	{
		writeStatus =  FPEC_CheckErrors();
	}

	FPEC_LockFlash();

	return writeStatus;
}





/**
  * @brief  Initialize the Embedded Flash Interface, unlock the flash and
  *         configure flash Latency according to system clock.
  * @note   This function must be called once before any reading, writing or erasing.
  * @param  None.
  * @retval Flash error.
  */
void  FPEC_InitFlash(void)
{
	RCC_EnablePeripheralClock(FLITF_PERIPHERAL);
	FPEC_UnlockFlash();

	/* Select flash Latency according to system clock */
#if  SYSTEM_CLK  <=  24000000
	CLR_BIT(FLASH->ACR, 0);
	CLR_BIT(FLASH->ACR, 1);
	CLR_BIT(FLASH->ACR, 2);
#elif  SYSTEM_CLK  <=  48000000
	SET_BIT(FLASH->ACR, 0);
	CLR_BIT(FLASH->ACR, 1);
	CLR_BIT(FLASH->ACR, 2);
#elif  SYSTEM_CLK  <=  72000000
	CLR_BIT(FLASH->ACR, 0);
	SET_BIT(FLASH->ACR, 1);
	CLR_BIT(FLASH->ACR, 2);
#endif

	FPEC_LockFlash();
}


/**
  * @brief  Erase a page from Embedded Flash memory.
  * @param  pageNumber: number of the page to be erased.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127)
  * @retval Flash error
  */
FPEC_Error_t  FPEC_ErasePage(u8 pageNumber)
{
	FPEC_Error_t  eraseStatus;

	FPEC_UnlockFlash();

	/* Wait if the flash is busy */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	SET_BIT(FLASH->CR, 1);   //Set bit 1 to select page erase

	/* Get address of first byte in the desired page to be erased */
	u32 pagePhysicalAddress = FLASH_BASE_ADDRESS + pageNumber * PAGE_SIZE_BYTES;
	FLASH->AR = pagePhysicalAddress;

	SET_BIT(FLASH->CR, 6);   //Start erasing the page

	/* wait until page erasing is completed */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	CLR_BIT(FLASH->CR, 1);   //Clear bit 1 to unselect page erase

	/* read first half word of this page to make sure it's erased successfully */
	if(FPEC_ReadHalfWord(pageNumber, 0) == 0xFFFF)
	{
		eraseStatus = FPEC_NO_ERRORS;
	}
	else
	{
		eraseStatus = FPEC_CheckErrors();
	}

	FPEC_LockFlash();

	return eraseStatus;
}



/**
  * @brief  Write a complete page in the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page where you want to write.
  * @param  data: array that contains actual data you want to write.
  * @retval Flash error.
  */
FPEC_Error_t  FPEC_WritePage(u8 pageNumber, u16* data)
{
	FPEC_Error_t writeStatus = FPEC_NO_ERRORS;

	FPEC_UnlockFlash();

	/* To write on a page, it must be erased first */
	writeStatus = FPEC_ErasePage(pageNumber);

	/* Write the data on the page by half word at a time */
	for(u16 i=0; i<PAGE_SIZE_HALF_WORDS; i++)
	{
		u16 offsetAddress = i * HALF_WORD_SIZE;
		writeStatus = FPEC_DirectWriteHalfWord(pageNumber, offsetAddress, data[i]);

		/* If there were errors while writing, don't continue writing */
		if(writeStatus != FPEC_NO_ERRORS)		break;
	}

	FPEC_LockFlash();

	return writeStatus;
}



/**
  * @brief  Read a complete page from the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page you want to read.
  * @param  data: the function will fill this array with the data stored in the desired page.
  * @retval Flash error.
  */
void  FPEC_ReadPage(u8 pageNumber, u16* data)
{
	/* Read the whole page by reading half word at a time */
	for(u16 i=0; i<PAGE_SIZE_HALF_WORDS; i++)
	{
		u16 offsetAddress = i * HALF_WORD_SIZE;
		data[i] = FPEC_ReadHalfWord(pageNumber, offsetAddress);
	}
}





/**
  * @brief  Read half word (2 bytes) from the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page you want to read from.
  * @param  offsetAddress: offset address of the half word you want to read.
  * @param  data: the function will fill this array with the data stored in the desired page.
  * @retval data stored in the desired half word.
  */
u16  FPEC_ReadHalfWord(u8 pageNumber, u16 offsetAddress)
{
	/* Wait if the flash is busy */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	/* Get physical address of the desired half word */
	volatile u16* physicalAddress = (volatile u16*)(FLASH_BASE_ADDRESS + pageNumber * PAGE_SIZE_BYTES + offsetAddress);

	/* Read data from this physical address */
	return *physicalAddress;
}



/**
  * @brief  Read byte from the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page you want to read from.
  * @param  offsetAddress: offset address of the half word you want to read.
  * @retval data stored in the desired byte.
  */
u8  FPEC_ReadByte(u8 pageNumber, u16 offsetAddress)
{
	/* Wait if the flash is busy */
	while(GET_BIT(FLASH->SR, 0) == 1) {}

	return (u8)FPEC_ReadHalfWord(pageNumber, offsetAddress);
}



/**
  * @brief  Read word (4 bytes) from the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page you want to read from.
  * @param  offsetAddress: offset address of the half word you want to read.
  * @retval data stored in the desired word.
  */
u32  FPEC_ReadWord(u8 pageNumber, u8  offsetAddress)
{
	u16 firstHalfWord = FPEC_ReadHalfWord(pageNumber, offsetAddress);
	u32 secndHalfWord = FPEC_ReadHalfWord(pageNumber, offsetAddress+2);
	return  (firstHalfWord | (secndHalfWord<<16));
}





/**
  * @brief  Write half word (2 bytes) in the flash memory.
  * @note   This function can be used without erasing the location you try to write on.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page where you want to write.
  * @param  offsetAddress: offset address of the half word you want to write.
  * @param  data: actual data you want to write.
  * @retval Flash error.
  */
FPEC_Error_t  FPEC_WriteHalfWord(u8 pageNumber, u16 offsetAddress, u16 data)
{
	/* Read the content of the desired page */
	u16 pageData[PAGE_SIZE_HALF_WORDS];
	FPEC_ReadPage(pageNumber, pageData);

	/* modify the desired half word */
	pageData[offsetAddress/2] = data;

	/* Erase page */
	FPEC_Error_t eraseStatus = FPEC_ErasePage(pageNumber);
	if(eraseStatus != FPEC_NO_ERRORS)
	{
		return eraseStatus;
	}

	/* Rewrite the page again after modifications */
	FPEC_Error_t writeStatus = FPEC_WritePage(pageNumber, pageData);
	if(writeStatus != FPEC_NO_ERRORS)
	{
		return writeStatus;
	}

	return FPEC_NO_ERRORS;
}



/**
  * @brief  Read word (4 bytes) from the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page where you want to write.
  * @param  offsetAddress: offset address of the half word you want to write.
  * @param  data: actual data you want to write.
  * @retval Flash error.
  */
FPEC_Error_t  FPEC_WriteWord(u8 pageNumber, u8  offsetAddress, u32 data)
{
	/* Read the content of the desired page */
	u16 pageData[PAGE_SIZE_HALF_WORDS];
	FPEC_ReadPage(pageNumber, pageData);

	/* modify the desired word */
	pageData[offsetAddress/2] = (u16)data;
	pageData[offsetAddress/2] = data>>16;

	/* Erase page */
	FPEC_Error_t eraseStatus = FPEC_ErasePage(pageNumber);
	if(eraseStatus != FPEC_NO_ERRORS)
	{
		return eraseStatus;
	}

	/* Rewrite the page again after modifications */
	FPEC_Error_t writeStatus = FPEC_WritePage(pageNumber, pageData);
	if(writeStatus != FPEC_NO_ERRORS)
	{
		return writeStatus;
	}

	return FPEC_NO_ERRORS;
}




/**
  * @brief  Write byte in the flash memory.
  * @note   There are 128 pages in the flash memory (from page 0 to page 127).
  * @note   In each page, there are 1024 bytes (from byte 0 to byte 1023).
  * @param  pageNumber: number of the page where you want to write.
  * @param  offsetAddress: offset address of the half word you want to write.
  * @param  data: actual data you want to write.
  * @retval Flash error.
  */
FPEC_Error_t  FPEC_WriteByte(u8 pageNumber, u16 offsetAddress, u8 data)
{
	/* Read the content of the desired page */
	u16 pageData[PAGE_SIZE_HALF_WORDS];
	FPEC_ReadPage(pageNumber, pageData);

	/* modify the desired byte */
	u16 originalhalfWord = pageData[offsetAddress/2];
	if(offsetAddress % 2 == 0)
	{
		originalhalfWord &= 0xFF00;
		originalhalfWord |= data;
	}
	else
	{
		originalhalfWord &= 0x00FF;
		originalhalfWord |= (((u16)data)<<8);
	}
	pageData[offsetAddress/2] = originalhalfWord;

	/* Erase page */
	FPEC_Error_t eraseStatus = FPEC_ErasePage(pageNumber);
	if(eraseStatus != FPEC_NO_ERRORS)
	{
		return eraseStatus;
	}

	/* Rewrite the page again after modifications */
	FPEC_Error_t writeStatus = FPEC_WritePage(pageNumber, pageData);
	if(writeStatus != FPEC_NO_ERRORS)
	{
		return writeStatus;
	}

	return FPEC_NO_ERRORS;
}






