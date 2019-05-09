#include <stdio.h>
#include <string.h>
#include "xhw_types.h"
#include "xhw_memmap.h"
#include "xhw_spi.h"
#include "xsysctl.h"
#include "xgpio.h"
#include "xuart.h"
#include "xspi.h"
#include "xcore.h"	// for SysTick

#include "socket.h"

#include "Internet/DNS/dns.h"

///////////////////////////////////
// STM32F103X SPI PIN Definition //
///////////////////////////////////
#define WIZCHIP_SPI_PERIPH    xSYSCTL_PERIPH_SPI1
#define WIZCHIP_SPI_BASE      xSPI1_BASE
#define WIZCHIP_SPI_CLK       SPI1CLK(3)
#define WIZCHIP_SPI_MOSI      SPI1MOSI(3)
#define WIZCHIP_SPI_MISO      SPI1MISO(1)
#define WIZCHIP_SPI_CLK_PIN   PA5
#define WIZCHIP_SPI_MOSI_PIN  PA7
#define WIZCHIP_SPI_MISO_PIN  PA6

/////////////////////////////////////////
// SOCKET NUMBER DEFINION for Examples //
/////////////////////////////////////////
#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define SOCK_DNS		6

////////////////////////////////////////////////
// Shared Buffer Definition for LOOPBACK TEST //
////////////////////////////////////////////////
#define DATA_BUF_SIZE   2048
uint8_t gDATABUF[DATA_BUF_SIZE];

///////////////////////////
// Network Configuration //
///////////////////////////

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {192, 168, 1, 123},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 1, 1},
                            .dns = {8, 8, 8, 8}, // Google public DNS Server (Primary)
                            .dhcp = NETINFO_STATIC };


/////////
// DNS //
/////////
uint8_t Domain_name[] = "www.google.com";
uint8_t secondary_dns_server[4] = {8, 8, 4, 4}; // Google public DNS Server (Secondary)
uint8_t ip_from_dns[4];

//////////////////////////////////////////////////////////////////////////////////////////////
// Call back function for W5500 SPI - Theses used to parameter or reg_wizchip_xxx_cbfunc()  //
//////////////////////////////////////////////////////////////////////////////////////////////
void  wizchip_select(void);
void  wizchip_deselect(void);
void  wizchip_write(uint8_t wb);
uint8_t wizchip_read();


void platform_init(void);
void network_init(void);

////////////////////////////////////////////////////////
// SysTick Interrupt Handler for DHCP timeout counter //
////////////////////////////////////////////////////////
uint32_t systick_counter;

void SysTickIntHandler(void)
{
	if (systick_counter++ > 1000) { // 1m x 1000 = 1sec
		systick_counter = 0;
		DNS_timerHandler(); // for DNS timeout counter;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Platform (STM32F103X) initialization for peripherals as GPIO, SPI, UARTs //
//////////////////////////////////////////////////////////////////////////////
void platform_init(void)
{
    xSysCtlClockSet(72000000,  xSYSCTL_OSC_MAIN | xSYSCTL_XTAL_8MHZ);

    xSysCtlDelay((xSysCtlClockGet()/1000)*50); // wait 50ms

    /****************/
	/* Init Systick */
	/****************/
    xSysTickPeriodSet(xSysCtlClockGet()/1000);

	xSysTickEnable();
	xSysTickIntEnable();


    /**************************/
    /* GPIO_A For SPI CS PIN  */// It must be first to enable GPIO peripheral than other peripheral. (Otherwise, UART do not run in STM32F103X)
    /**************************/
    xSysCtlPeripheralEnable( xSYSCTL_PERIPH_GPIOA );
    xGPIODirModeSet(xGPIO_PORTA_BASE, xGPIO_PIN_4, xGPIO_DIR_MODE_OUT);


    /************/
    /* For UART */
    /************/
    xSysCtlPeripheralReset(xSYSCTL_PERIPH_UART1);
    xSysCtlPeripheralEnable(xSYSCTL_PERIPH_UART1);
    xSPinTypeUART(UART1TX,PA9);
    xSPinTypeUART(UART1RX,PA10);
    xUARTConfigSet(xUART1_BASE, 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    xUARTEnable(xUART1_BASE, (UART_BLOCK_UART | UART_BLOCK_TX | UART_BLOCK_RX));


    /***********/
    /* For SPI */
    /***********/
    xSysCtlPeripheralReset(WIZCHIP_SPI_PERIPH);
    xSysCtlPeripheralEnable(WIZCHIP_SPI_PERIPH);
    xSPinTypeSPI(WIZCHIP_SPI_CLK, WIZCHIP_SPI_CLK_PIN);  // xGPIODirModeSet(xGPIO_PORTA, xGPIO_PIN_5, GPIO_TYPE_AFOUT_STD, GPIO_OUT_SPEED_50M);
    xSPinTypeSPI(WIZCHIP_SPI_MOSI,WIZCHIP_SPI_MOSI_PIN);  // xGPIODirModeSet(xGPIO_PORTA, xGPIO_PIN_7, GPIO_TYPE_AFOUT_STD, GPIO_OUT_SPEED_50M);
    xSPinTypeSPI(WIZCHIP_SPI_MISO,WIZCHIP_SPI_MISO_PIN);  // xGPIODirModeSet(xGPIO_PORTA, xGPIO_PIN_6, GPIO_TYPE_IN_FLOATING, GPIO_IN_SPEED_FIXED);
    xSPIConfigSet(WIZCHIP_SPI_BASE, xSysCtlClockGet()/2, xSPI_MOTO_FORMAT_MODE_0 | xSPI_MODE_MASTER | xSPI_MSB_FIRST | xSPI_DATA_WIDTH8);

    xSPISSSet(WIZCHIP_SPI_BASE, SPI_SS_SOFTWARE, xSPI_SS_NONE);
    xSPIEnable(WIZCHIP_SPI_BASE);

    printf("HCLK = %dMHz\r\n", (unsigned int)(xSysCtlClockGet()/1000000));

}


void network_init(void)
{
   uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
		  gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	printf("======================\r\n");
}


int main(void)
{
   uint8_t tmp;
   uint8_t memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

   platform_init();

   /////////////////////////////////////////////
   // Register SPI callback function of W5500 //
   /////////////////////////////////////////////

   /* Critical section callback - No use in this example */
   //reg_wizchip_cris_cbfunc(0, 0);

   /* Chip selection call back */
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_select);  // CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
    /////////////////////////////////////////////


    /* wizchip initialize*/
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
       printf("WIZCHIP Initialized fail.\r\n");
       while(1);
    }

    /* PHY link status check */
    do
    {
       if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
          printf("Unknown PHY Link stauts.\r\n");
    }while(tmp == PHY_LINK_OFF);

    /* Network initialization */
    network_init();

	/************************************************/
	/* WIZnet W5500 Code Examples : 				*/
	/* Implemented using ioLibrary_BSD Socket APIs	*/
	/************************************************/
	/* >> DNS Client 								*/
	/************************************************/

    /* DNS client example */
	printf("\r\n=== DNS Client Example ===============\r\n");
	printf("> Primary DNS Server : %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
	printf("> Secondary DNS Server : %d.%d.%d.%d\r\n", secondary_dns_server[0], secondary_dns_server[1], secondary_dns_server[2], secondary_dns_server[3]);
	printf("======================================\r\n");
	printf("> Example Domain Name : %s\r\n", Domain_name);

	/* DNS client initialization */
	DNS_init(SOCK_DNS, gDATABUF);

	/* DNS query & response */
	tmp = DNS_run(gWIZNETINFO.dns, secondary_dns_server, Domain_name, ip_from_dns);
	//tmp = DNS_run(gWIZNETINFO.dns, 0, Domain_name, ip_from_dns); // if you want to use only primary DNS server, second parameter set to 0

	if (tmp > 0) {
		printf("> DNS Query Success - %s\r\n", (tmp == 1) ? "Primary DNS Server":"Secondary DNS Server");
		printf(">> IP address of [%s] : %d.%d.%d.%d\r\n",Domain_name, ip_from_dns[0], ip_from_dns[1], ip_from_dns[2], ip_from_dns[3]);
	} else {
		printf(">> DNS Failed\r\n");
	}
	printf("======================================\r\n");

    /* Main loop */
    while(1)
	{
    	;
	} // end of Main loop
} // end of main()


void  wizchip_select(void)
{
   xGPIOPinWrite( xGPIO_PORTA_BASE, xGPIO_PIN_4, 0);
}

void  wizchip_deselect(void)
{
   xGPIOPinWrite( xGPIO_PORTA_BASE, xGPIO_PIN_4, 1);
}

void  wizchip_write(uint8_t wb)
{
   xSPISingleDataReadWrite(WIZCHIP_SPI_BASE,wb);
}

uint8_t wizchip_read()
{
   return xSPISingleDataReadWrite(WIZCHIP_SPI_BASE,0xFF);
}
