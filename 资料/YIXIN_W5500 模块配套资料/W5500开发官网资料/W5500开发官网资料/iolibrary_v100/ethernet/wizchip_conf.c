//****************************************************************************/ 
//!
//! \file wizchip_conf.c
//! \brief WIZCHIP Config Header File.
//! \version 1.0.0.0
//! \date 09/01/2013
//! \par  Revision history
//!       <09-01-2013> 1st Release
//! \author MidnightCow
//! \copy
//!
//! Copyright (c)  2013, WIZnet Co., LTD.
//! All rights reserved.
//! 
//! Redistribution and use in source and binary forms, with or without 
//! modification, are permitted provided that the following conditions 
//! are met: 
//! 
//!     * Redistributions of source code must retain the above copyright 
//! notice, this list of conditions and the following disclaimer. 
//!     * Redistributions in binary form must reproduce the above copyright
//! notice, this list of conditions and the following disclaimer in the
//! documentation and/or other materials provided with the distribution. 
//!     * Neither the name of the <ORGANIZATION> nor the names of its 
//! contributors may be used to endorse or promote products derived 
//! from this software without specific prior written permission. 
//! 
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
//! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
//! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
//! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
//! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
//! THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************/
#include "wizchip_conf.h"
/**
 * @brief Default function to enable interrupt.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	  wizchip_cris_enter(void)           {};
/**
 * @brief Default function to disable interrupt.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	  wizchip_cris_exit(void)          {};
/**
 * @brief Default function to select chip.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	wizchip_cs_select(void)            {};
/**
 * @brief Default function to deselect chip.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	wizchip_cs_deselect(void)          {};
/**
 * @brief Default function to read in direct or indirect interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
uint8_t wizchip_bus_readbyte(uint32_t AddrSel) { return * ((volatile uint8_t *) AddrSel); };
/**
 * @brief Default function to write in direct or indirect interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
 
void 	wizchip_bus_writebyte(uint32_t AddrSel, uint8_t wb)  { *((volatile uint8_t*)AddrSel) = wb; };
/**
 * @brief Default function to read in SPI interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
uint8_t wizchip_spi_readbyte(void)        {return 0;};
/**
 * @brief Default function to write in SPI interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	wizchip_spi_writebyte(uint8_t wb) {};


_WIZCHIP  WIZCHIP =
      {
      .id                  = _WIZCHIP_ID_,
      .if_mode             = _WIZCHIP_IO_MODE_,
      .CRIS._enter         = wizchip_cris_enter,
      .CRIS._exit          = wizchip_cris_exit,
      .CS._select          = wizchip_cs_select,
      .CS._deselect        = wizchip_cs_deselect,
      .IF.BUS._read_byte   = wizchip_bus_readbyte,
      .IF.BUS._write_byte  = wizchip_bus_writebyte
//    .IF.SPI._read_byte   = wizchip_spi_readbyte,
//    .IF.SPI._write_byte  = wizchip_spi_writebyte
      };

//static uint8_t    _DNS_[4];      // DNS server ip address
//static dhcp_mode  _DHCP_;        // DHCP mode

void reg_wizchip_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void))
{
   if(!cris_en || !cris_ex)
   {
      WIZCHIP.CRIS._enter = wizchip_cris_enter;
      WIZCHIP.CRIS._exit  = wizchip_cris_exit;
   }
   else
   {
      WIZCHIP.CRIS._enter = cris_en;
      WIZCHIP.CRIS._exit  = cris_ex;
   }
}

void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void))
{
   if(!cs_sel || !cs_desel)
   {
      WIZCHIP.CS._select   = wizchip_cs_select;
      WIZCHIP.CS._deselect = wizchip_cs_deselect;
   }
   else
   {
      WIZCHIP.CS._select   = cs_sel;
      WIZCHIP.CS._deselect = cs_desel;
   }
}

void reg_wizchip_bus_cbfunc(uint8_t(*bus_rb)(uint32_t addr), void (*bus_wb)(uint32_t addr, uint8_t wb))
{
   while(!(WIZCHIP.if_mode & _WIZCHIP_IO_MODE_BUS_));
   
   if(!bus_rb || !bus_wb)
   {
      WIZCHIP.IF.BUS._read_byte   = wizchip_bus_readbyte;
      WIZCHIP.IF.BUS._write_byte  = wizchip_bus_writebyte;
   }
   else
   {
      WIZCHIP.IF.BUS._read_byte   = bus_rb;
      WIZCHIP.IF.BUS._write_byte  = bus_wb;
   }
}

void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb))
{
   while(!(WIZCHIP.if_mode & _WIZCHIP_IO_MODE_SPI_));
   
   if(!spi_rb || !spi_wb)
   {
      WIZCHIP.IF.SPI._read_byte   = wizchip_spi_readbyte;
      WIZCHIP.IF.SPI._write_byte  = wizchip_spi_writebyte;
   }
   else
   {
      WIZCHIP.IF.SPI._read_byte   = spi_rb;
      WIZCHIP.IF.SPI._write_byte  = spi_wb;
   }
}

void wizchip_sw_reset(void)
{
   uint8_t gw[4], sn[4], sip[4];
   uint8_t mac[6];
   getSHAR(mac);
   getGAR(gw);  getSUBR(sn);  getSIPR(sip);
   setMR(MR_RST);
   getMR(); // for delay
   setSHAR(mac);
   setGAR(gw);
   setSUBR(sn);
   setSIPR(sip);
}

int8_t wizchip_init(uint8_t* txsize, uint8_t* rxsize)
{
   int8_t i;
   int8_t tmp = 0;
   wizchip_sw_reset();
   if(txsize)
   {
      tmp = 0;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         tmp += txsize[i];
      if(tmp > 16) return -1;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         setSn_TXBUF_SIZE(i, txsize[i]);
   }
   if(rxsize)
   {
      tmp = 0;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         tmp += rxsize[i];
      if(tmp > 16) return -1;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         setSn_RXBUF_SIZE(i, rxsize[i]);
   }
   return 0;
}

