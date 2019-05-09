//*****************************************************************************
//
//! \file wizchip_conf.h
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
//*****************************************************************************

#ifndef  _WIZCHIP_CONF_H_
#define  _WIZCHIP_CONF_H_

#include <stdint.h>
/**
 * @brief Select WIZCHIP.
 * @todo You should select one, \b 5100, \b 5200 or \b 5500.
 */
#define _WIZCHIP_                      5500   // 5100, 5200, 5500

#define _WIZCHIP_IO_MODE_NONE_         0x0000
#define _WIZCHIP_IO_MODE_BUS_          0x0100 /**< Bus interface mode */
#define _WIZCHIP_IO_MODE_SPI_          0x0200 /**< SPI interface mode */

#define _WIZCHIP_IO_MODE_BUS_DIR_      (_WIZCHIP_IO_MODE_BUS_ + 1) /**< BUS interface mode for direct  */
#define _WIZCHIP_IO_MODE_BUS_INDIR_    (_WIZCHIP_IO_MODE_BUS_ + 2) /**< BUS interface mode for indirect */

#define _WIZCHIP_IO_MODE_SPI_VDM_      (_WIZCHIP_IO_MODE_SPI_ + 1) /**< SPI interface mode for variable length data*/
#define _WIZCHIP_IO_MODE_SPI_FDM_      (_WIZCHIP_IO_MODE_SPI_ + 2) /**< SPI interface mode for fixed length data mode*/


#if (_WIZCHIP_ == 5500)
/**
 * @brief Define chip ID.
 */
  #define _WIZCHIP_ID_                 "W5500\0"
/**
 * @brief Define interface mode.
 * @todo You should select interface mode as chip.
 */
//   #define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_SPI_FDM_
   #define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_SPI_VDM_

   #include "W5500/w5500.h"
#else 
   #error "Unknown defined _WIZCHIP_. You should define one of 5100, 5200, and 5500 !!!"
#endif

#ifndef _WIZCHIP_IO_MODE_
   #error "Undefined _WIZCHIP_IO_MODE_. You should define it !!!"
#endif

/*
 * @brief Define I/O base address when BUS IF mode.
 * @todo Must re-define it to fit your system when BUS IF Mode.
 */

//#define _WIZCHIP_IO_BASE_              0x00000000  // TODO : Must re-define it to fit your system when BUS IF Mode.

#if _WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS
   #ifndef _WIZCHIP_IO_BASE_
      #error "You should be define _WIZCHIP_IO_BASE to fit your system memory map."
   #endif
#endif   

#if _WIZCHIP_ > 5100
   #define _WIZCHIP_SOCK_NUM_   8
#else
   #define _WIZCHIP_SOCK_NUM_   4
#endif      


#define DEFAULT_MAC_ADDR   "00:08:DC:70:80:22"
#define DEFAULT_IP_ADDR    "192.168.11.123"
#define DEFAULT_SN_MASK    "255.255.255.0"
#define DEFAULT_GW_ADDR    "192.168.11.1"
#define DEFAULT_DNS_ADDR   "168.126.63.1"

/*********************************************************
* WIZCHIP BASIC IF functions for SPI, SDIO, I2C , ETC.
*********************************************************/
typedef struct __WIZCHIP
{
   uint16_t  if_mode;
   uint8_t   id[6];
   struct _CRIS
   {
      void (*_enter)  (void);
      void (*_exit) (void);   
   }CRIS;  
   struct _CS
   {
      void (*_select)  (void);
      void (*_deselect)(void);
   }CS;  
   union 
   {	   
      struct
      {
         uint8_t  (*_read_byte)  (uint32_t AddrSel);
         void     (*_write_byte) (uint32_t AddrSel, uint8_t wb);
      }BUS;      
      struct
      {
         uint8_t (*_read_byte)   (void);
         void    (*_write_byte)  (uint8_t wb);
      }SPI;
      // To be added
      //
   }IF;
}_WIZCHIP;

extern _WIZCHIP  WIZCHIP;


/**
 *@brief Registers call back function for critical section of I/O functions such as
 *\ref WIZCHIP_READ, @ref WIZCHIP_WRITE, @ref WIZCHIP_READ_BUF and @ref WIZCHIP_WRITE_BUF.
 *@param cris_en : callback function for critical section enter.
 *@param cris_ex : callback function for critical section exit.
 *@todo Describe @ref wizchip_int_enable and @ref wizchip_int_disable function or register your functions.
 *@note If you do not describe or register, default functions(@ref wizchip_cris_enter & @ref wizchip_cris_exit) is called.
 */
void reg_wizchip_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void));


/**
 *@brief Registers call back function for WIZCHIP select & deselect.
 *@param cs_sel : callback function for WIZCHIP select
 *@param cs_desel : callback fucntion for WIZCHIP deselect
 *@todo Describe @ref wizchip_cs_select and @ref wizchip_cs_deselect function or register your functions.
 *@note If you do not describe or register, null function is called.
 */
void reg_wizchip_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void));

/**
 *@brief Registers call back function for bus interface.
 *@param bus_rb   : callback function to read byte data using system bus
 *@param bus_wb   : callback function to write byte data using system bus
 *@todo Describe @ref wizchip_bus_readbyte and @ref wizchip_bus_writebyte function
 *or register your functions.
 *@note If you do not describe or register, null function is called.
 */
void reg_wizchip_bus_cbfunc(uint8_t (*bus_rb)(uint32_t addr), void (*bus_wb)(uint32_t addr, uint8_t wb));

/**
 *@brief Registers call back function for SPI interface.
 *@param spi_rb : callback function to read byte usig SPI 
 *@param spi_wb : callback function to write byte usig SPI 
 *@todo Describe \ref wizchip_spi_readbyte and \ref wizchip_spi_writebyte function
 *or register your functions.
 *@note If you do not describe or register, null function is called.
 */
void reg_wizchip_spi_cbfunc(uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb));


/**
 * Reset WIZCHIP by softly.
 */ 
void   wizchip_sw_reset(void);

/**
 * Initializes WIZCHIP with socket buffer size
 * @param txsize - socket tx buffer sizes. if null, initialized the default size 2KB.
 * @param rxsize - socket rx buffer sizes. if null, initialized the default size 2KB.
 * @return 0 : succcess
 *        -1 : fail. Invalid buffer size
 */
int8_t wizchip_init(uint8_t* txsize, uint8_t* rxsize);


#endif   // _WIZCHIP_CONF_H_
