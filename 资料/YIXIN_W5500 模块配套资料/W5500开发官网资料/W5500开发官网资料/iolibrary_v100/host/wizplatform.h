/**
 * @file		wizplatform.h
 * @brief		Platform Utility Common Header File
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 * @author	Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef	_WIZPLATFORM_H
#define	_WIZPLATFORM_H

/**
 * @addtogroup usart_module
 * @{
 */
/** Indicate the USART index number */
typedef enum {
	WIZ_USART1,				///< Indicate the 1st USART
	WIZ_USART2,				///< Indicate the 2nd USART
	//WIZ_USART3				//< Indicate the 3rd USART
} wizpf_usart;

/** Indicate the USART Baud Rate */
typedef enum usart_baudrate_t {
	UBR_110 = 110,			///< Baudrate 110 bps
	UBR_300 = 300,			///< Baudrate 300 bps
	UBR_600 = 600,			///< Baudrate 600 bps
	UBR_1200 = 1200,		///< Baudrate 1200 bps
	UBR_2400 = 2400,		///< Baudrate 2400 bps
	UBR_4800 = 4800,		///< Baudrate 4800 bps
	UBR_9600 = 9600,		///< Baudrate 9600 bps
	UBR_14400 = 14400,		///< Baudrate 14400 bps
	UBR_19200 = 19200,		///< Baudrate 19200 bps
	UBR_38400 = 38400,		///< Baudrate 38400 bps
	UBR_57600 = 57600,		///< Baudrate 57600 bps
	UBR_115200 = 115200,	///< Baudrate 115200 bps
	UBR_230400 = 230400,	///< Baudrate 230400 bps
	UBR_460800 = 460800,	///< Baudrate 460800 bps
	UBR_921600 = 921600		///< Baudrate 921600 bps
} usart_baudrate;

/** Indicate the USART Word Length */
typedef enum usart_wordlen_t {
	UWL_8    = 0,					///< Word Length 8 bit
	UWL_9    = 1					///< Word Length 9 bit
} usart_wordlen;

/** Indicate the USART Stop Bit */
typedef enum usart_stopbit_t {
	UST_0d5  = 0 ,				///< Stop bit 0.5 bit
	UST_1    = 1 ,					///< Stop bit 1 bit
	UST_1d5  = 2 ,				///< Stop bit 1.5 bit
	UST_2    = 3					///< Stop bit 2 bit
} usart_stopbit;

/** Indicate the USART Parity Bit */
typedef enum usart_parity_t {
	UPB_NO   = 0,					///< No Parity
	UPB_EVEN = 1,				///< Even Parity
	UPB_ODD  = 2					///< Odd Parity
} usart_parity;

/** Indicate the USART Flow Control */
typedef enum usart_flowcon_t {
	UFC_NO   = 0 ,				///< No Flow Control
	UFC_HW   = 1					///< RTS+CTS Flow Control
	//UFC_SW ,				//< Xon/Xoff Flow Control
} usart_flowcon;

/** USART Parameter struct used by @ref wizpf_usart_init */
typedef struct usart_param_t {
	uint32_t baudrate;				///< Baud Rate (bps in decimal value, of @ref usart_baudrate_t)
	usart_wordlen wordlen;			///< Word Length (of @ref usart_wordlen_t)
	usart_stopbit stopbit;			///< Stop Bit (of @ref usart_stopbit_t)
	usart_parity parity;			///< Parity Bit (of @ref usart_parity_t)
	usart_flowcon flowcon;			///< Flow Control (of @ref usart_flowcon_t)
} usart_param;
/* @} */

/**
 * @ingroup spi_module
 * Indicate the SPI index number
 */
typedef enum {
	WIZ_SPI1 = 0,	///< Indicate the 1st SPI
	WIZ_SPI2 = 1, 	///< Indicate the 2nd SPI
	WIZ_SPI3 = 2	///< Indicate the 3rd SPI
} wizpf_spi;

/**
 * @ingroup platform_util_module
 * Indicate the LED index number
 */
typedef enum {
	WIZ_LED1,				///< Indicate the 1st LED
	WIZ_LED2,				///< Indicate the 2nd LED
	WIZ_LED3,				///< Indicate the 3rd LED
	WIZ_LED4				///< Indicate the 4th LED
} wizpf_led;

/**
 * @ingroup gpio_module
 * Indicate the GPIO mode
 */
typedef enum {
	GMOD_IN_FLOAT,			///< Indicate Floating Input
	GMOD_IN_PULLUP,			///< Indicate Pulled up Input
	GMOD_IN_PULLDOWN,		///< Indicate Pulled down Input
	GMOD_OUT_PUSHPULL,		///< Indicate Push-Pull Output
	GMOD_OUT_OPENDRAIN,		///< Indicate Open-Drain Output
	GMOD_AF_PUSHPULL,		///< Indicate Push-Pull Alternate Function
	GMOD_AF_OPENDRAIN		///< Indicate Open-Drain Alternate Function
} gpio_mode;

/**
 * @def WIZPF_USART_SET_PARAM
 * @ingroup usart_module
 * Set USART Parameters to @ref usart_param_t pointer
 * @param param_p USART Parameter struct pointer
 * @param br_e Baud Rate to set (@ref usart_baudrate_t)
 * @param wl_e Word Length to set (@ref usart_wordlen_t)
 * @param sb_e Stop Bit to set (@ref usart_stopbit_t)
 * @param pb_e Parity Bit to set (@ref usart_parity_t)
 * @param fc_e Flow Control to set (@ref usart_flowcon_t)
 */
#define WIZPF_USART_SET_PARAM(param_p, br_e, wl_e, sb_e, pb_e, fc_e) do { \
	(param_p)->baudrate = br_e; \
	(param_p)->wordlen = wl_e; \
	(param_p)->stopbit = sb_e; \
	(param_p)->parity = pb_e; \
	(param_p)->flowcon = fc_e; \
} while(0)

/**
 * @def wizpf_led_flicker
 * @ingroup platform_util_module
 * Flicker a LED for debug with some interval.
 * @param led_v LED Index number (@ref wizpf_led)
 * @param interval_v Interval time (ms)
 */
#define wizpf_led_flicker(led_v, interval_v) do { \
	static uint32_t tick = 0; \
	if(wizpf_tick_elapse(tick) > interval_v) { \
		wizpf_led_set(led_v, VAL_TOG); \
		tick = wizpf_get_systick(); \
	} \
} while(0)

int8_t platform_init(usart_param *up);
int8_t wizpf_spi_init(wizpf_spi spi);

uint32_t wizpf_get_systick(void);
uint32_t wizpf_tick_conv(uint8_t istick2sec, uint32_t tickorsec);
int32_t wizpf_tick_elapse(uint32_t tick);
void Delay_us(uint8_t time_us);
void Delay_ms(uint16_t time_ms);
void Delay_tick(uint32_t tick);

int8_t wizpf_led_set(wizpf_led led, uint8_t action);
int8_t wizpf_led_get(wizpf_led led);
void wizpf_led_trap(uint8_t repeat);

int32_t wizpf_putchar(int32_t ch, wizpf_usart usart);
int32_t wizpf_getchar(wizpf_usart usart);
int32_t wizpf_getcharNB(wizpf_usart usart);

void wizchip_HW_reset(void);
//call back function for WIZCHIP
void wizpf_wizchip_cris_enter();
void wizpf_wizchip_cris_exit();
void wizpf_wizchip_select();
void wizpf_wizchip_deselect();
void wizpf_wizchip_spi_writebyte(uint8_t wb );
uint8_t wizpf_wizchip_spi_readbyte();

#endif //_WIZPLATFORM_H



