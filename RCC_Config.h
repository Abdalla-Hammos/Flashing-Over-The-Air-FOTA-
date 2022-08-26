#ifndef  RCC_CONFIG_H_
#define  RCC_CONFIG_H_


/*  clock sources:
 *     - HSI --> high speed internal clock (8 MHz)
 *     - HSE --> high speed external clock (must be in range 4 - 16 MHz)
 *     - PLL --> phase locked loop (used to multiply the frequency)
 * 
 *  HSE can be:
 *     - RC circuit
 *     - XTAL crystal oscillator
 * 
 *  PLL input clock can be:
 *     - HSI/2
 *     - HSE
 *     - HSE/2
 * 
 *  output clock from PLL is its input clock multiplied by a factor
 *  PLL multiplication factor can be 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 or 16
 *
 ********* NOTE: MAX ALLOWED OUTPUT CLOCK FROM PLL IS 72 MHz ***********
 * 
 *  clock is provided to peripherals through one of the following busses
 *     - AHB  --> max frequency is 72 MHz
 *     - APB1 --> max frequency is 36 MHz
 *     - APB2 --> max frequency is 72 MHz
 * 
 *  system clock is devided by a factor then it's feed to AHB bus, this factor can be
 *     - 1   --> AHB clock = system clock
 *     - 2   --> AHB clock = system clock / 2
 *     - 4   --> AHB clock = system clock / 4
 *     - 8   --> AHB clock = system clock / 8
 *     - 16  --> AHB clock = system clock / 16
 *     - 64  --> AHB clock = system clock / 64
 *     - 128 --> AHB clock = system clock / 128
 *     - 256 --> AHB clock = system clock / 256
 *     - 512 --> AHB clock = system clock / 512
 * 
 *  AHB clock is devided by a factor then it's feed to APB1 bus, this factor can be
 *     - 1   --> APB1 clock = AHB clock
 *     - 2   --> APB1 clock = AHB clock / 2
 *     - 4   --> APB1 clock = AHB clock / 4
 *     - 8   --> APB1 clock = AHB clock / 8
 *     - 16  --> APB1 clock = AHB clock / 16
 * 
 *  AHB clock is devided by a factor then it's feed to APB2 bus, this factor can be
 *     - 1   --> APB2 clock = AHB clock
 *     - 2   --> APB2 clock = AHB clock / 2
 *     - 4   --> APB2 clock = AHB clock / 4
 *     - 8   --> APB2 clock = AHB clock / 8
 *     - 16  --> APB2 clock = AHB clock / 16
 * 
 ********* NOTE: MAX ALLOWED CLOCK FOR APB1 IS 36 MHz ***********
 *
 */


/* HSI --> high speed internal (8 MHz), its state can be ENABLED or DISABLED */
#define  HSI_STATE      ENABLED

/* HSE --> high speed external, its state can be ENABLED or DISABLED */
#define  HSE_STATE      ENABLED

/* PLL --> phase locked loop, its state can be ENABLED or DISABLED */
#define  PLL_STATE      ENABLED

/* HSE_SRC --> HSE source, it can be HSE_RC or HSE_XTAL */
#define  HSE_SRC        HSE_XTAL

/* HSE_VAL --> HSE clock frequency in Hz, it must be in range 4 - 16 MHz */
#define  HSE_VAL        8000000UL

/* PLL_SRC --> input clock to PLL, it can be HSI_DIVIDED_BY_2 or HSE or HSE_DIVIDED_BY_2 */
#define  PLL_SRC        HSE

/* PLL_FACTOR --> PLL multiplication factor, it can be: 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 or 16
 ********* NOTE: MAX ALLOWED OUTPUT CLOCK FROM PLL IS 72 MHz ************/
#define  PLL_FACTOR     9

/* CLK_TYPE --> input clock to CPU, it can be HSI or HSE or PLL */
#define  CLK_TYPE       PLL

/* CLK_SECURITY_SYSTEM --> if HSE is used and suddenly failed, clk security system can detect it and switch to HSI 
 * its state can be ENABLED or DISABLED */
#define  CLK_SECURITY_SYSTEM_STATE    ENABLED

/* AHB_DIVISION_FACTOR  --> division factor of system clock to get AHB clock
 * it can be 1 or 2 or 4 or 8 or 16 or 64 or 128 or 256 or 512 */
#define  AHB_DIVISION_FACTOR      1

/* APB1_DIVISION_FACTOR --> division factor of AHB clock to get APB1 clock
 * it can be 1 or 2 or 4 or 8 or 16
 ********* NOTE: MAX ALLOWED CLOCK FOR APB1 IS 36 MHz ************/
#define  APB1_DIVISION_FACTOR     2

/* APB2_DIVISION_FACTOR --> division factor of AHB clock to get APB2 clock
 * it can be 1 or 2 or 4 or 8 or 16 */
#define  APB2_DIVISION_FACTOR     1


#endif
