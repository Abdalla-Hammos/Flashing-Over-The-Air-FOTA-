#ifndef  RCC_H_
#define  RCC_H_

#include "stm32f10x.h"
#include "RCC_Config.h"
#include "bitMainuplation.h"
#include "stdint.h"
#include "FPEC.h"

#define  DISABLED   1
#define  ENABLED    2

/* clock sources */
#define  HSI   1     //high speed internal clock (8 MHz)
#define  HSE   2     //high speed external clock (must be in range 4 - 16 MHz)
#define  PLL   3     //phase locked loop

/* HSE sources */
#define  HSE_RC      1    //external clock from RC oscillator
#define  HSE_XTAL    2    //external clock from crystal oscillator

/* PLL sources */
#define  HSI_DIVIDED_BY_2    4
#define  HSE_DIVIDED_BY_2    5

/* buses */
#define  AHB_BUS       1
#define  APB1_BUS      2
#define  APB2_BUS      3


/* AHB peripherals */
#define  DMA1_PERIPHERAL         0
#define  DMA2_PERIPHERAL         1
#define  SRAM_PERIPHERAL         2
#define  FLITF_PERIPHERAL        4
#define  CRC_PERIPHERAL          6
#define  FSMC_PERIPHERAL         8
#define  SDIO_PERIPHERAL         10

/* APB1 peripherals */
#define  TIM2_PERIPHERAL         32
#define  TIM3_PERIPHERAL         33
#define  TIM4_PERIPHERAL         34
#define  TIM5_PERIPHERAL         35
#define  TIM6_PERIPHERAL         36
#define  TIM7_PERIPHERAL         37
#define  TIM12_PERIPHERAL        38
#define  TIM13_PERIPHERAL        39
#define  TIM14_PERIPHERAL        40
#define  WWDG_PERIPHERAL         43
#define  SPI2_PERIPHERAL         46
#define  SPI3_PERIPHERAL         47
#define  USART2_PERIPHERAL       49
#define  USART3_PERIPHERAL       50
#define  UART4_PERIPHERAL        51
#define  UART5_PERIPHERAL        52
#define  I2C1_PERIPHERAL         53
#define  I2C2_PERIPHERAL         54
#define  USB_PERIPHERAL          55
#define  CAN_PERIPHERAL          57
#define  BKP_PERIPHERAL          59
#define  PWR_PERIPHERAL          60
#define  DAC_PERIPHERAL          61

/* APB2 peripherals */
#define  AFIO_PERIPHERAL         64
#define  GPIOA_PERIPHERAL        66
#define  GPIOB_PERIPHERAL        67
#define  GPIOC_PERIPHERAL        68
#define  GPIOD_PERIPHERAL        69
#define  GPIOE_PERIPHERAL        70
#define  GPIOF_PERIPHERAL        71
#define  GPIOG_PERIPHERAL        72
#define  ADC1_PERIPHERAL         73
#define  ADC2_PERIPHERAL         74
#define  TIM1_PERIPHERAL         75
#define  SPI1_PERIPHERAL         76
#define  TIM8_PERIPHERAL         77
#define  USART1_PERIPHERAL       78
#define  ADC3_PERIPHERAL         79
#define  TIM9_PERIPHERAL         83
#define  TIM10_PERIPHERAL        84
#define  TIM11_PERIPHERAL        85





#define  HSI_VAL           (8000000UL)
#define  MAX_SYSTEM_CLK    (72000000UL)
#define  HSE_MAX_CLK       (16000000UL)
#define  HSE_MIN_CLK       (4000000UL)
#define  APB1_MAX_CLK      (36000000UL)


/* SYSTEM_CLK --> input clock to CPU */
#if  CLK_TYPE == HSI
	#define  SYSTEM_CLK     HSI_VAL
#elif  CLK_TYPE == HSE
	#define  SYSTEM_CLK     HSE_VAL
#elif  CLK_TYPE == PLL
	#if  PLL_SRC == HSI_DIVIDED_BY_2
		#define  SYSTEM_CLK     (HSI_VAL * PLL_FACTOR / 2)
	#elif  PLL_SRC == HSE
		#define  SYSTEM_CLK     (HSE_VAL * PLL_FACTOR)
	#elif  PLL_SRC == HSE_DIVIDED_BY_2
		#define  SYSTEM_CLK     (HSE_VAL * PLL_FACTOR / 2)
	#else
		#error  "wrong PLL_SRC, it must be one of the following --> HSI_DIVIDED_BY_2 , HSE , HSE_DIVIDED_BY_2"
	#endif
#else
	#error  "wrong CLK_TYPE, it must be one of the following --> HSI , HSE , PLL"
#endif




/* busses clock */
#define  AHB_CLK    (SYSTEM_CLK / AHB_DIVISION_FACTOR )
#define  APB1_CLK   (  AHB_CLK  / APB1_DIVISION_FACTOR)
#define  APB2_CLK   (  AHB_CLK  / APB2_DIVISION_FACTOR)





/* AHB peripherals clock */
#define  DMA1_CLK         AHB_CLK
#define  DMA2_CLK         AHB_CLK
#define  SRAM_CLK         AHB_CLK
#define  FLITF_CLK        AHB_CLK
#define  CRC_CLK          AHB_CLK
#define  FSMC_CLK         AHB_CLK
#define  SDIO_CLK         AHB_CLK

/* APB1 peripherals clock */
#define  TIM2_CLK         APB1_CLK
#define  TIM3_CLK         APB1_CLK
#define  TIM4_CLK         APB1_CLK
#define  TIM5_CLK         APB1_CLK
#define  TIM6_CLK         APB1_CLK
#define  TIM7_CLK         APB1_CLK
#define  TIM12_CLK        APB1_CLK
#define  TIM13_CLK        APB1_CLK
#define  TIM14_CLK        APB1_CLK
#define  WWDG_CLK         APB1_CLK
#define  SPI2_CLK         APB1_CLK
#define  SPI3_CLK         APB1_CLK
#define  USART2_CLK       APB1_CLK
#define  USART3_CLK       APB1_CLK
#define  UART4_CLK        APB1_CLK
#define  UART5_CLK        APB1_CLK
#define  I2C1_CLK         APB1_CLK
#define  I2C2_CLK         APB1_CLK
#define  USB_CLK          APB1_CLK
#define  CAN_CLK          APB1_CLK
#define  BKP_CLK          APB1_CLK
#define  PWR_CLK          APB1_CLK
#define  DAC_CLK          APB1_CLK

/* APB2 peripherals clock */
#define  AFIO_CLK         APB2_CLK
#define  GPIOA_CLK        APB2_CLK
#define  GPIOB_CLK        APB2_CLK
#define  GPIOC_CLK        APB2_CLK
#define  GPIOD_CLK        APB2_CLK
#define  GPIOE_CLK        APB2_CLK
#define  GPIOF_CLK        APB2_CLK
#define  GPIOG_CLK        APB2_CLK
#define  ADC1_CLK         APB2_CLK
#define  ADC2_CLK         APB2_CLK
#define  TIM1_CLK         APB2_CLK
#define  SPI1_CLK         APB2_CLK
#define  TIM8_CLK         APB2_CLK
#define  USART1_CLK       APB2_CLK
#define  ADC3_CLK         APB2_CLK
#define  TIM9_CLK         APB2_CLK
#define  TIM10_CLK        APB2_CLK
#define  TIM11_CLK        APB2_CLK




void  RCC_InitClock(void);
void  RCC_EnablePeripheralClock (uint8_t peripheral);
void  RCC_DisablePeripheralClock(uint8_t peripheral);
void  RCC_PeripheralReset(uint8_t peripheral);   //can be used for one time only for each peripheral



#endif
