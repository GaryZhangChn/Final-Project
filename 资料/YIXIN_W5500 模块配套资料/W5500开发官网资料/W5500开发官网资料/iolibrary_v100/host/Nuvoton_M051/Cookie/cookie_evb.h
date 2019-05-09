/**
 * @file		cookie_evb.h
 * @brief		cookie_evb Evaluation Board Header File
 * @version	1.0
 * @date		2013/09/01
 * @par Revision
 *			2013/09/01 - 1.0 Release
 * @author	EunKyoung
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef _COOKIE_EVB
#define _COOKIE_EVB

#include "cookie.h"

// GPIO PIN Definition - Common

// Port
#define PORTA					GPIO_PORTA_BASE			///< GPIO Group A
#define PORTB					GPIO_PORTB_BASE			///< GPIO Group B
#define PORTC					GPIO_PORTC_BASE			///< GPIO Group C
// Pin	
#define PIN0					GPIO_PIN_0		         ///< GPIO Pin 0
#define PIN1					GPIO_PIN_1					///< GPIO Pin 1
#define PIN2					GPIO_PIN_2					///< GPIO Pin 2
#define PIN3					GPIO_PIN_3					///< GPIO Pin 3
#define PIN4					GPIO_PIN_4					///< GPIO Pin 4
#define PIN5					GPIO_PIN_5					///< GPIO Pin 5
#define PIN6					GPIO_PIN_6					///< GPIO Pin 6
#define PIN7					GPIO_PIN_7					///< GPIO Pin 7
#define PIN8					GPIO_PIN_8					///< GPIO Pin 8
#define PIN9					GPIO_PIN_9					///< GPIO Pin 9
#define PIN10					GPIO_PIN_10					///< GPIO Pin 10
#define PIN11					GPIO_PIN_11					///< GPIO Pin 11
#define PIN12					GPIO_PIN_12					///< GPIO Pin 12
#define PIN13					GPIO_PIN_13					///< GPIO Pin 13
#define PIN14					GPIO_PIN_14					///< GPIO Pin 14
#define PIN15					GPIO_PIN_15					///< GPIO Pin 15

// GPIO PIN Definition - Platform Specific

#define LED1_PORT				GPIO_PORTB_BASE			///< 1st LED Port
#define LED1_PIN				GPIO_PIN_1		         ///< 1st LED Pin
#define LED2_PORT				GPIO_PORTB_BASE			///< 2nd LED Port
#define LED2_PIN				GPIO_PIN_2					///< 2nd LED Pin
#define LED1_GPIO    	   sA1
#define LED2_GPIO    	   sA2
	
	
#define UART1_TX_PORT		GPIO_PORTA_BASE		///< 1st UART Tx Port
#define UART1_TX_PIN			GPIO_PIN_2				///< 1st UART Tx Pin
#define UART1_RX_PORT		GPIO_PORTA_BASE		///< 1st UART Rx Port
#define UART1_RX_PIN			GPIO_PIN_3				///< 1st UART Rx Pin
	
	
#define UART2_TX_PORT		GPIO_PORTA_BASE		///< 2nd UART Tx Port
#define UART2_TX_PIN			GPIO_PIN_2				///< 2nd UART Tx Pin
#define UART2_RX_PORT		GPIO_PORTA_BASE		///< 2nd UART RX Port
#define UART2_RX_PIN			GPIO_PIN_3				///< 2nd UART RX Pin


#define SPI0_SCS_PORT		GPIO_PORTA_BASE		///< 2nd SPI SCS Port
#define SPI0_SCS_PIN		   GPIO_PIN_4				///< 2nd SPI SCS Pin
#define SPI0_SCLK_PORT		GPIO_PORTB_BASE		///< 2nd SPI SCLK Port
#define SPI0_SCLK_PIN		GPIO_PIN_7				///< 2nd SPI SCLK Pin
#define SPI0_MISO_PORT		GPIO_PORTB_BASE		///< 2nd SPI MISO Port
#define SPI0_MISO_PIN		GPIO_PIN_6				///< 2nd SPI MISO Pin
#define SPI0_MOSI_PORT		GPIO_PORTB_BASE		///< 2nd SPI MOSI Port
#define SPI0_MOSI_PIN		GPIO_PIN_5				///< 2nd SPI MOSI Pin

#define SPI1_SCS_PORT		GPIO_PORTA_BASE		///< 1st SPI SCS Port
#define SPI1_SCS_PIN		   GPIO_PIN_4				///< 1st SPI SCS Pin
#define SPI1_SCLK_PORT		GPIO_PORTA_BASE		///< 1st SPI SCLK Port
#define SPI1_SCLK_PIN		GPIO_PIN_7				///< 1st SPI SCLK Pin
#define SPI1_MISO_PORT		GPIO_PORTA_BASE		///< 1st SPI MISO Port
#define SPI1_MISO_PIN		GPIO_PIN_6				///< 1st SPI MISO Pin
#define SPI1_MOSI_PORT		GPIO_PORTA_BASE		///< 1st SPI MOSI Port
#define SPI1_MOSI_PIN		GPIO_PIN_5				///< 1st SPI MOSI Pin


//Cookie

#define SPI0_SCS_GPIO      sD10
#define SPI0_SCLK_GPIO     sICSP_3_SCK
#define SPI0_MOSI_GPIO     sICSP_4_MOSI
#define SPI0_MISO_GPIO     sICSP_1_MISO
                           
#define SPI1_SCS_GPIO      sD10
#define SPI1_SCLK_GPIO     sD13
#define SPI1_MOSI_GPIO     sD11
#define SPI1_MISO_GPIO     sD12
                           
#define SPI1_LED1_GPIO     sA1
#define SPI1_LED2_GPIO     sA2

#define WIZ_INT_PORT			GPIO_PORTB_BASE		///< Network Device Interrupt Port
#define WIZ_INT_PIN			GPIO_PIN_0				///< Network Device Interrupt Pin
#define WIZ_PWDN_PORT		GPIO_PORTB_BASE		///< Network Device PowerDown Port
#define WIZ_PWDN_PIN			GPIO_PIN_9				///< Network Device PowerDown Pin

#endif //_COOKIE_EVB



