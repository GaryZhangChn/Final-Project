/**
 * @file		wizplatform.c
 * @brief		Platform Specific Source File - For W5500 Evaluation Board
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 * @author	EunKyoung
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

//#define FILE_LOG_SILENCE
#include "common/common.h"

#define USART1_RX_INTERRUPT VAL_DISABLE//VAL_DISABLE//VAL_ENABLE
#define USART2_RX_INTERRUPT VAL_DISABLE//VAL_DISABLE//VAL_ENABLE
#define SYSTICK_HZ			1000


uint32_t msTicks = 0;	// Max: about 50 days
void SysTickIntHandler(void)	// SysTick ISR
{
   msTicks++;

}


#if (USART1_RX_INTERRUPT == VAL_ENABLE)
#define U0RX_BUF_SIZE		16
uint8_t  u0rx_buf[U0RX_BUF_SIZE];
uint16_t u0rx_wr=0, u0rx_rd=0;
void USART0_IRQHandler(void)	// USART1 ISR
{
	if(UARTIntStatus(xUART0_BASE, xUART_INT_RX) != RESET) {
		UARTIntClear(xUART0_BASE, xUART_INT_RX);
		if( (u0rx_wr > u0rx_rd && u0rx_wr-u0rx_rd >= U0RX_BUF_SIZE-1) ||
			(u0rx_wr < u0rx_rd && u0rx_rd == u0rx_wr+1) )	// Buffer Overflow
		{
			
			UARTCharPut(xUART0_BASE, (uint8_t)'@');
			return;
		}
		u0rx_buf[u0rx_wr] = (uint8_t)UARTCharGet(xUART0_BASE);
		if(u0rx_wr < U0RX_BUF_SIZE-1) u0rx_wr++;
		else u0rx_wr = 0;
	}
}
#endif

#if (USART2_RX_INTERRUPT == VAL_ENABLE)
#define U1RX_BUF_SIZE		16
uint8_t  u1rx_buf[U1RX_BUF_SIZE];
uint16_t u1rx_wr=0, u1rx_rd=0;
void USART1_IRQHandler(void)	// USART2 ISR
{
	if(UARTIntStatus(xUART1_BASE, xUART_INT_RX) != RESET) {
		UARTIntClear(xUART1_BASE, xUART_INT_RX);
		if( (u1rx_wr > u1rx_rd && u1rx_wr-u1rx_rd >= U1RX_BUF_SIZE-1) ||
			(u1rx_wr < u1rx_rd && u1rx_rd == u1rx_wr+1) )	// Buffer Overflow
		{
			
			UARTCharPut(xUART1_BASE, (uint8_t)'@');
			return;
		}
		u1rx_buf[u1rx_wr] = (uint8_t)UARTCharGet(xUART1_BASE);
		if(u1rx_wr < U1RX_BUF_SIZE-1) u1rx_wr++;
		else u1rx_wr = 0;
	}
}
#endif
int8_t platform_init(usart_param *up)
{
	int8_t ret8;

	uint8_t tx_size[8]={2, 2, 2, 2, 2, 2, 2, 2}; // Device default memory setting
	uint8_t rx_size[8]={2, 2, 2, 2, 2, 2, 2, 2};

	xSysCtlClockSet(12000000,  xSYSCTL_OSC_MAIN | xSYSCTL_XTAL_12MHZ);
	
	xSysCtlDelay(44000000);
	xSysTickPeriodSet(xSysCtlClockGet()/1000);

	// GPIOA enable
	xSysCtlPeripheralEnable(xSYSCTL_PERIPH_GPIOA );
	xSysCtlPeripheralEnable(xSYSCTL_PERIPH_GPIOB );
	
	xSysCtlPeripheralReset(xSYSCTL_PERIPH_UART0);
	xSysCtlPeripheralEnable(xSYSCTL_PERIPH_UART0);
	  
	xGPIODirModeSet(LED1_PORT, LED1_PIN,xGPIO_DIR_MODE_OUT);// xGPIO_DIR_MODE_QB);//xGPIO_DIR_MODE_OUT);
	xGPIODirModeSet(LED2_PORT, LED2_PIN,xGPIO_DIR_MODE_OUT);// xGPIO_DIR_MODE_QB);//xGPIO_DIR_MODE_OUT);
	//xGPIOSPinTypeGPIOOutputQB(LED1_GPIO);
	//xGPIOSPinTypeGPIOOutputQB(LED2_GPIO);
	xSPinTypeUART(UART0TX, PA2);
	xSPinTypeUART(UART0RX, PA3);

	if(up == NULL) {
		xUARTConfigSet(xUART0_BASE, 115200, (UART_CONFIG_WLEN_8 |
                                         UART_CONFIG_STOP_ONE |
                                         UART_CONFIG_PAR_NONE));

	}
	else
   {
		xUARTConfigSet(xUART0_BASE, up->baudrate, (UART_CONFIG_WLEN_8 |
                                         UART_CONFIG_STOP_ONE |
                                         UART_CONFIG_PAR_NONE));
   }
#if (USART1_RX_INTERRUPT == VAL_ENABLE)
	xUARTIntCallbackInit(xUART0_BASE,USART0_IRQHandler );
#endif
#if (USART2_RX_INTERRUPT == VAL_ENABLE)
	xUARTIntCallbackInit(xUART1_BASE,USART1_IRQHandler );
#endif

   xUARTEnable(xUART0_BASE, (UART_BLOCK_UART | UART_BLOCK_TX | UART_BLOCK_RX));
 	
	ret8 = wizpf_spi_init(WIZ_SPI1);
	if(ret8 != RET_OK) {
		ERR("ERR: wizpf_spi_init fail");
		return RET_NOK;
	}

	////////////////////////////////////////////////////////////////////////////////
	// reg_wizchip_cris_cbfunc(wizpf_wizchip_cris_enter,wizpf_wizchip_cris_exit); // If fail to the critical section process, comment this statement and test again.
	////////////////////////////////////////////////////////////////////////////////
#if (_WIZCHIP_IO_MODE_== _WIZCHIP_IO_MODE_SPI_FDM_)
	reg_wizchip_cs_cbfunc(wizpf_wizchip_select, wizpf_wizchip_select);    // For being tied to the low.
#else
	reg_wizchip_cs_cbfunc(wizpf_wizchip_select, wizpf_wizchip_deselect);
#endif
   reg_wizchip_spi_cbfunc(wizpf_wizchip_spi_readbyte, wizpf_wizchip_spi_writebyte);
	wizchip_HW_reset();

	if(wizchip_init(tx_size,rx_size) < 0)
	{
	      ERR("ERR: WIZCHIP fail to initialized!!!");
	      while(1);
	}

	wizpf_led_set(WIZ_LED1, VAL_ON);
	wizpf_led_set(WIZ_LED2, VAL_ON);


#if (USART1_RX_INTERRUPT == VAL_ENABLE)
   xUARTIntEnable(xUART0_BASE,xUART_INT_RX);
#endif

   xSysTickEnable();
   xSysTickIntEnable();

	return RET_OK;
}



void wizchip_HW_reset(void)
{
#ifdef WIZ_RESET_PIN
	xGPIOPinWrite( WIZ_RESET_PORT, WIZ_RESET_PIN, 0);
	Delay_us(8);
	xGPIOPinWrite( WIZ_RESET_PORT, WIZ_RESET_PIN, 1);
	Delay_ms(50);
#endif
}

void Delay_us(uint8_t time_us)
{
	register uint8_t i, j;

	for(i=0; i<time_us; i++) {
		for(j=0; j<5; j++) {	// 25CLK
			asm("nop");	//1CLK         
			asm("nop");	//1CLK         
			asm("nop");	//1CLK         
			asm("nop");	//1CLK         
			asm("nop");	//1CLK                  
		}      
	}					// 25CLK*0.04us=1us
}

void Delay_ms(uint16_t time_ms)
{
	register uint16_t i;

	for(i=0; i<time_ms; i++) {
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
	}
}


void Delay_tick(uint32_t tick)
{

   uint32_t curTicks;
    curTicks = msTicks;
   while(msTicks - curTicks < tick);
}

uint32_t wizpf_get_systick(void)
{
   return msTicks;
}

uint32_t wizpf_tick_conv(uint8_t istick2sec, uint32_t tickorsec)
{
   if(istick2sec) return tickorsec / SYSTICK_HZ;   // tick to seconds
   else return tickorsec * SYSTICK_HZ; // seconds to tick
}

int32_t wizpf_tick_elapse(uint32_t tick)   // + Elapsed time, - Remaining time
{
   uint32_t cur = wizpf_get_systick();

   return cur - tick;
}


/**
 * @addtogroup spi_module
 * @{
 */

/**
 * Initialize SPI Peripheral Device.
 * @param spi SPI index number (@ref wizpf_spi)
 * @return RET_OK: Success
 * @return RET_NOK: Error
 */
int8_t wizpf_spi_init(wizpf_spi spi)
{
	switch(spi) {
		case WIZ_SPI1:
	   xSysCtlPeripheralReset(xSYSCTL_PERIPH_SPI0);
	   xSysCtlPeripheralEnable(xSYSCTL_PERIPH_SPI0);
	   xGPIODirModeSet(xGPIO_PORTA_BASE, xGPIO_PIN_4, xGPIO_DIR_MODE_OUT);

	   xSPinTypeSPI(SPI0CLK, SPI0_SCLK_GPIO);
	   xSPinTypeSPI(SPI0MOSI,SPI0_MOSI_GPIO);
	   xSPinTypeSPI(SPI0MISO,SPI0_MISO_GPIO);
	   xSPIConfigSet(xSPI0_BASE, 18000000, xSPI_MOTO_FORMAT_MODE_0 | xSPI_MODE_MASTER | xSPI_MSB_FIRST | xSPI_DATA_WIDTH8);

	   xSPISSSet(xSPI0_BASE, SPI_SS_SOFTWARE, xSPI_SS_NONE);

	   xSPIEnable(xSPI0_BASE);
  		break;
	case WIZ_SPI2:
	   xSysCtlPeripheralReset(xSYSCTL_PERIPH_SPI1);
	   xSysCtlPeripheralEnable(xSYSCTL_PERIPH_SPI1);
	   xGPIODirModeSet(xGPIO_PORTA_BASE, xGPIO_PIN_4, xGPIO_DIR_MODE_OUT);
	
		
	   xSPinTypeSPI(SPI1CLK, SPI1_SCLK_GPIO);
	   xSPinTypeSPI(SPI1MOSI,SPI1_MOSI_GPIO);
	   xSPinTypeSPI(SPI1MISO,SPI1_MISO_GPIO);

	   xSPIConfigSet(xSPI1_BASE, 18000000, xSPI_MOTO_FORMAT_MODE_0 | xSPI_MODE_MASTER | xSPI_MSB_FIRST | xSPI_DATA_WIDTH8);
	   xSPISSSet(xSPI1_BASE, SPI_SS_SOFTWARE, xSPI_SS_NONE);

	   xSPIEnable(xSPI1_BASE);
		
   break;


	default:
		return RET_NOK;
	}
	return RET_OK;
}


int8_t wizpf_led_set(wizpf_led led, uint8_t action)
{
	uint32_t GPIO_Port = 0;
	uint32_t GPIO_Pin = 0;

	switch(led) {
	case WIZ_LED1:
		GPIO_Port = LED1_PORT;
		GPIO_Pin = LED1_PIN;
		break;

	case WIZ_LED2:
		GPIO_Port = LED2_PORT;
		GPIO_Pin = LED2_PIN;
		break;

	default:
		action = VAL_TOG + 1;
		break;
	}

	if(action == VAL_OFF) {
		xGPIOPinWrite( GPIO_Port, GPIO_Pin, 0);// LED off
	} else if(action == VAL_ON) {
		xGPIOPinWrite( GPIO_Port, GPIO_Pin, 1);// LED on
	} else if(action == VAL_TOG) {
	   if(wizpf_led_get(led)!=0)// if(xGPIOPinRead(GPIO_Port, GPIO_Pin)!=0)
	      xGPIOPinWrite( GPIO_Port, GPIO_Pin, 0);
	   else
	      xGPIOPinWrite( GPIO_Port, GPIO_Pin, 1);
	} else return RET_NOK;

	return RET_OK;
}

int8_t wizpf_led_get(wizpf_led led)
{
	uint32_t GPIO_Port = 0;
	uint32_t GPIO_Pin = 0;

	switch(led) {

	case WIZ_LED1:
		GPIO_Port = LED1_PORT;
		GPIO_Pin = LED1_PIN;
		break;
	case WIZ_LED2:
		GPIO_Port = LED2_PORT;
		GPIO_Pin = LED2_PIN;
		break;
	default:
	   return RET_NOK;
	}
	return (int8_t)xGPIOPinRead(GPIO_Port,GPIO_Pin);
}

void wizpf_led_trap(uint8_t repeat)
{
   uint32_t i=0;
#ifdef LED1_PIN
	wizpf_led_set(WIZ_LED1, VAL_OFF);
	while(1) {
		Delay_ms(1500);
		for(i=0; i<repeat; i++) {
			wizpf_led_set(WIZ_LED1, VAL_ON);
			Delay_ms(200);
			wizpf_led_set(WIZ_LED1, VAL_OFF);
			Delay_ms(200);
		}
	}
#endif
}


void wizpf_wizchip_cris_enter()
{
   if(!xIntMasterEnable())
   {
      ERR("Fail to enter critical section for WIZCHIP\r\n");
      while(1);
   }
}
void wizpf_wizchip_cris_exit()
{
   if(!xIntMasterDisable())
   {
      ERR("Fail to exit critical section for WIZCHIP\r\n");
      while(1);
   }
}

void wizpf_wizchip_select()
{
   xGPIOPinWrite( SPI0_SCS_PORT, SPI0_SCS_PIN, 0);
}

void wizpf_wizchip_deselect()
{
   xGPIOPinWrite( SPI0_SCS_PORT, SPI0_SCS_PIN, 1);
}

void wizpf_wizchip_spi_writebyte(uint8_t wb )
{
   xSPISingleDataReadWrite(xSPI0_BASE,wb);
}

uint8_t wizpf_wizchip_spi_readbyte()
{
   return xSPISingleDataReadWrite(xSPI0_BASE,0xFF);
}


int32_t wizpf_putchar(int32_t ch, wizpf_usart usart)
{
  uint32_t USARTx;

  if(usart == WIZ_USART1) USARTx = xUART0_BASE;
  else if(usart == WIZ_USART2) USARTx = xUART1_BASE;
  else return RET_NOK;

  UARTCharPut(USARTx, (uint8_t)ch);
  return ch;
}

int32_t wizpf_getchar(wizpf_usart usart)
{
#if (USART1_RX_INTERRUPT == VAL_ENABLE)
  if(usart == WIZ_USART1) {
     while(u0rx_rd == u0rx_wr);
     if(u0rx_rd < U0RX_BUF_SIZE-1) {
        u0rx_rd++;
        return u0rx_buf[u0rx_rd-1];
     } else {
        u0rx_rd = 0;
        return u0rx_buf[U0RX_BUF_SIZE-1];
     }
  }
#endif

#if (USART2_RX_INTERRUPT == VAL_ENABLE)
  if(usart == WIZ_USART2) {
     while(u1rx_rd == u1rx_wr);
     if(u1rx_rd < U1RX_BUF_SIZE-1) {
        u1rx_rd++;
        return u1rx_buf[u1rx_rd-1];
     } else {
        u1rx_rd = 0;
        return u1rx_buf[U1RX_BUF_SIZE-1];
     }
  }
#endif

#if (USART1_RX_INTERRUPT == VAL_DISABLE) || (USART2_RX_INTERRUPT == VAL_DISABLE)
  {
  uint32_t USARTx;

  if(usart == WIZ_USART1) USARTx = xUART0_BASE;
  else if(usart == WIZ_USART2) USARTx = xUART1_BASE;
  else return RET_NOK;
  return UARTCharGet(USARTx);
  }
#else
  return RET_NOK;
#endif
}

int32_t wizpf_getcharNB(wizpf_usart usart)
{
#if (USART1_RX_INTERRUPT == VAL_ENABLE)
  if(usart == WIZ_USART1) {
     if(u0rx_rd == u0rx_wr) return RET_NOK;
     if(u0rx_rd < U0RX_BUF_SIZE-1) {
        u0rx_rd++;
        return u0rx_buf[u0rx_rd-1];
     } else {
        u0rx_rd = 0;
        return u0rx_buf[U0RX_BUF_SIZE-1];
     }
  }
#endif

#if (USART2_RX_INTERRUPT == VAL_ENABLE)
  if(usart == WIZ_USART2) {
     if(u1rx_rd == u1rx_wr) return RET_NOK;
     if(u1rx_rd < U1RX_BUF_SIZE-1) {
        u1rx_rd++;
        return u1rx_buf[u1rx_rd-1];
     } else {
        u1rx_rd = 0;
        return u1rx_buf[U1RX_BUF_SIZE-1];
     }
  }
#endif

#if (USART1_RX_INTERRUPT == VAL_DISABLE) || (USART2_RX_INTERRUPT == VAL_DISABLE)
  {
   uint32_t USARTx;

  if(usart == WIZ_USART1) USARTx = xUART0_BASE;
  else if(usart == WIZ_USART2) USARTx = xUART1_BASE;
  else return RET_NOK;
  return UARTCharGetNonBlocking(USARTx);
  }
#else
  return RET_NOK;
#endif
}
