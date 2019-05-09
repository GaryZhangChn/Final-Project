//*****************************************************************************
//
//! \file w5500.c
//! \brief W5500 HAL Interface.
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
#include <stdio.h>
#include "w5500.h"


#define _W5500_SPI_VDM_OP_          0x00
#define _W5500_SPI_FDM_OP_LEN1_     0x01
#define _W5500_SPI_FDM_OP_LEN2_     0x02
#define _W5500_SPI_FDM_OP_LEN4_     0x03

////////////////////////////////////////////////////

void     WIZCHIP_CRITICAL_ENTER(void)
{
   WIZCHIP.CRIS._enter();
}

void     WIZCHIP_CRITICAL_EXIT(void)
{
   WIZCHIP.CRIS._exit();
}
         
uint8_t  WIZCHIP_READ(uint32_t AddrSel)
{
   uint8_t ret;

   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
   	   AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
   	   AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_FDM_OP_LEN1_);
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
   ret = WIZCHIP.IF.SPI._read_byte();

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5000. !!!"   
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
   return ret;
}

void     WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb )
{
	WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
   	   AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
   	   AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_FDM_OP_LEN1_);
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
   WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
   WIZCHIP.IF.SPI._write_byte(wb);

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}
         
void     WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
   uint16_t i = 0;
   uint16_t j = 0;
   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
      AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
      for(i = 0; i < len; i++, j)
        pBuf[i] = WIZCHIP.IF.SPI._read_byte();
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
      AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_FDM_OP_LEN4_);
      for(i = 0; i < len/4; i++)
      {
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
         pBuf[i*4]   = WIZCHIP.IF.SPI._read_byte();
         pBuf[i*4+1] = WIZCHIP.IF.SPI._read_byte();
         pBuf[i*4+2] = WIZCHIP.IF.SPI._read_byte();            
         pBuf[i*4+3] = WIZCHIP.IF.SPI._read_byte();            
         //AddrSel += (4 << 8);    // offset address + 4
         AddrSel = WIZCHIP_OFFSET_INC(AddrSel,4);
      }
      len %= 4;      // for the rest data
      if(len)
      {
         AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN4_ to _W5500_SPI_FDM_OP_LEN2_
         i *= 4;
         for(j = 0; j < len/2 ; j++)
         {
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            pBuf[i]   = WIZCHIP.IF.SPI._read_byte();
            pBuf[i+1] = WIZCHIP.IF.SPI._read_byte();
            i += 2;
            //AddrSel += (2 << 8);    // offset address + 2
            AddrSel = WIZCHIP_OFFSET_INC(AddrSel,2);
         }
         len %= 2;
         if(len)
         {
            AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN2_ to _W5500_SPI_FDM_OP_LEN1_
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            pBuf[i]   = WIZCHIP.IF.SPI._read_byte();
         }      
      }
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}

void     WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
   uint16_t i = 0;
   uint16_t j = 0;
   WIZCHIP_CRITICAL_ENTER();
   WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
      AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
      for(i = 0; i < len; i++, j)
         WIZCHIP.IF.SPI._write_byte(pBuf[i]);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
      AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_FDM_OP_LEN4_);
      for(i = 0; i < len/4; i++)
      {
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
         WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4]  );
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4+1]);
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4+2]);            
         WIZCHIP.IF.SPI._write_byte(pBuf[i*4+3]);            
         //AddrSel += (4 << 8);    // offset address + 4
         AddrSel = WIZCHIP_OFFSET_INC(AddrSel,4);
      }
      len %= 4;      // for the rest data
      if(len)
      {
         AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN4_ to _W5500_SPI_FDM_OP_LEN2_
         i *= 4;
         for(j = 0; j < len/2 ; j++)
         {
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            WIZCHIP.IF.SPI._write_byte(pBuf[i]  );
            WIZCHIP.IF.SPI._write_byte(pBuf[i+1]);
            i += 2;
            //AddrSel += (2 << 8);    // offset address + 2
            AddrSel = WIZCHIP_OFFSET_INC(AddrSel, 2);
         }
         len %= 2;
         if(len)
         {
            AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN2_ to _W5500_SPI_FDM_OP_LEN1_
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
            WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
            WIZCHIP.IF.SPI._write_byte(pBuf[i]);
         }      
      }
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!!"
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
}


/////////////////////////////////
// Common Register IO function //
/////////////////////////////////

void     setMR(uint8_t mr)
{
   WIZCHIP_WRITE(MR,mr);
}

uint8_t  getMR(void)
{
   return WIZCHIP_READ(MR);
}

void     setGAR(uint8_t* gar)
{
   WIZCHIP_WRITE_BUF(GAR,gar,4);
}

void     getGAR(uint8_t* gar)
{
   WIZCHIP_READ_BUF(GAR,gar,4);
}
         
void     setSUBR(uint8_t * subr)
{
   WIZCHIP_WRITE_BUF(SUBR, subr,4);
}

void     getSUBR(uint8_t * subr)
{
   WIZCHIP_READ_BUF(SUBR, subr, 4);
}
         
void     setSHAR(uint8_t * shar)
{
   WIZCHIP_WRITE_BUF(SHAR, shar, 6);
}

void     getSHAR(uint8_t * shar)
{
   WIZCHIP_READ_BUF(SHAR, shar, 6);
}
         
void     setSIPR(uint8_t * sipr)
{
   WIZCHIP_WRITE_BUF(SIPR, sipr, 4);
}

void     getSIPR(uint8_t * sipr)
{
   WIZCHIP_READ_BUF(SIPR, sipr, 4);
}

void     setINTLEVEL(uint16_t intlevel)
{
   WIZCHIP_WRITE(INTLEVEL,   (uint8_t)(intlevel >> 8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(INTLEVEL,1), (uint8_t) intlevel);
}

uint16_t getINTLEVEL(void)
{
   uint16_t intlv = 0;
   intlv = WIZCHIP_READ(INTLEVEL);
   intlv = (intlv << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(INTLEVEL,1));
   return intlv;
}

void     setIR(uint8_t ir)
{
   WIZCHIP_WRITE(IR, (ir & 0xF0));
}

uint8_t  getIR(void)
{
   return (WIZCHIP_READ(IR) & 0xF0);
}

void     setIMR(uint8_t imr)
{
   WIZCHIP_WRITE(IMR, imr);
}

uint8_t  getIMR(void)
{
   return WIZCHIP_READ(IMR);
}

void     setSIR(uint8_t sir)
{
   WIZCHIP_WRITE(SIR, sir);
}

uint8_t  getSIR(void)
{
   return WIZCHIP_READ(SIR);
}

void     setSIMR(uint8_t simr)
{
   WIZCHIP_WRITE(SIMR, simr);
}

uint8_t  getSIMR(void)
{
   return WIZCHIP_READ(SIMR);
}

void     setRTR(uint16_t rtr)
{
   WIZCHIP_WRITE(RTR,   (uint8_t)(rtr >> 8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(RTR,1), (uint8_t) rtr);
}                     

uint16_t getRTR(void)
{
   uint16_t rtr;
   rtr = WIZCHIP_READ(RTR);
   rtr = (rtr << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(RTR,1));
   return rtr;
}

void     setRCR(uint8_t rcr)
{
   WIZCHIP_WRITE(RCR, rcr);   
}

uint8_t  getRCR(void)
{
   return WIZCHIP_READ(RCR);
}

void     setPTIMER(uint8_t ptimer)
{
   WIZCHIP_WRITE(PTIMER, ptimer);
}

uint8_t  getPTIMER(void)
{
   return WIZCHIP_READ(PTIMER);
}

void     setPMAGIC(uint8_t pmagic)
{
   WIZCHIP_WRITE(PMAGIC, pmagic);
}

uint8_t  getPMAGIC(void)
{
   return WIZCHIP_READ(PMAGIC);
}

void     setPHAR(uint8_t* phar)
{
   WIZCHIP_WRITE_BUF(PHAR, phar, 6);
}

void     getPHAR(uint8_t* phar)
{
   WIZCHIP_READ_BUF(PHAR, phar, 6);
}

void     setPSID(uint16_t psid)
{
   WIZCHIP_WRITE(PSID,   (uint8_t)(psid >> 8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(PSID,1), (uint8_t) psid);
}

uint16_t  getPSID(void)
{
   uint16_t psid;
   psid = WIZCHIP_READ(PSID);
   psid = (psid << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(PSID,1));
   return psid;
}

void     setPMRU(uint16_t pmru)
{
   WIZCHIP_WRITE(PMRU,   (uint8_t)(pmru>>8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(PMRU,1), (uint8_t) pmru);
}

uint16_t getPMRU(void)
{
   uint16_t pmru;
   pmru = WIZCHIP_READ(PMRU);
   pmru = (pmru << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(PMRU,1));
   return pmru;
}

//
void     getUIPR(uint8_t* uipr)
{
   WIZCHIP_READ_BUF(UIPR,uipr,6);
}

uint16_t getUPORTR(void)
{
   uint16_t uportr;
   uportr = WIZCHIP_READ(UPORTR);
   uportr = (uportr << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(UPORTR,1));
   return uportr;
}
void     setPHYCFGR(uint8_t phycfgr)
{
   WIZCHIP_WRITE(PHYCFGR, phycfgr);
}

uint8_t  getPHYCFGR(void)
{
   return WIZCHIP_READ(PHYCFGR);
}

uint8_t  getVERSIONR(void)
{
   return WIZCHIP_READ(VERSIONR);
}
/////////////////////////////////////

///////////////////////////////////
// Socket N regsiter IO function //
///////////////////////////////////
void     setSn_MR(uint8_t sn, uint8_t mr)
{
   WIZCHIP_WRITE(Sn_MR(sn),mr);
}

uint8_t  getSn_MR(uint8_t sn)
{
   return WIZCHIP_READ(Sn_MR(sn));
}

void     setSn_CR(uint8_t sn, uint8_t cr)
{
   WIZCHIP_WRITE(Sn_CR(sn), cr);
}

uint8_t  getSn_CR(uint8_t sn)
{
   return WIZCHIP_READ(Sn_CR(sn));
}

void     setSn_IR(uint8_t sn, uint8_t ir)
{
   WIZCHIP_WRITE(Sn_IR(sn), ir);
}

uint8_t  getSn_IR(uint8_t sn)
{
   return WIZCHIP_READ(Sn_IR(sn));
}

void     setSn_IMR(uint8_t sn, uint8_t imr)
{
   WIZCHIP_WRITE(Sn_IMR(sn), (imr & 0xF0));
}

uint8_t  getSn_IMR(uint8_t sn)
{
   return (WIZCHIP_READ(Sn_IMR(sn)) & 0xF0);
}

//
uint8_t  getSn_SR(uint8_t sn)
{
   return WIZCHIP_READ(Sn_SR(sn));
}

void     setSn_PORT(uint8_t sn, uint16_t port)
{
   WIZCHIP_WRITE(Sn_PORT(sn),   (uint8_t)(port >> 8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_PORT(sn),1), (uint8_t) port);
}

uint16_t getSn_PORT(uint8_t sn)
{
   uint16_t port;
   port = WIZCHIP_READ(Sn_PORT(sn));
   port = (port << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_PORT(sn),1));
   return port;
}
          
void     setSn_DHAR(uint8_t sn, uint8_t* dhar)
{
   WIZCHIP_WRITE_BUF(Sn_DHAR(sn), dhar, 6);
}

void     getSn_DHAR(uint8_t sn, uint8_t* dhar)
{
   WIZCHIP_READ_BUF(Sn_DHAR(sn), dhar, 6);
}

void     setSn_DIPR(uint8_t sn, uint8_t* dipr)
{
   WIZCHIP_WRITE_BUF(Sn_DIPR(sn), dipr, 4);
}

void     getSn_DIPR(uint8_t sn, uint8_t* dipr)
{
   WIZCHIP_READ_BUF(Sn_DIPR(sn), dipr, 4);
}

void     setSn_DPORT(uint8_t sn, uint16_t dport)
{
   WIZCHIP_WRITE(Sn_DPORT(sn),   (uint8_t) (dport>>8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_DPORT(sn),1), (uint8_t)  dport);
}

uint16_t getSn_DPORT(uint8_t sn)
{
   uint16_t port;
   port = WIZCHIP_READ(Sn_DPORT(sn));
   port = (port<<8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_DPORT(sn),1));
   return port;
}

void     setSn_MSSR(uint8_t sn, uint16_t mss)
{
   WIZCHIP_WRITE(Sn_MSSR(sn),   (uint8_t)(mss>>8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_MSSR(sn),1), (uint8_t) mss);
}

uint16_t getSn_MSSR(uint8_t sn)
{
   uint16_t mss;
   mss = WIZCHIP_READ(Sn_MSSR(sn));
   mss = (mss << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_MSSR(sn),1));
   return mss;
}

void     setSn_TOS(uint8_t sn, uint8_t tos)
{
   WIZCHIP_WRITE(Sn_TOS(sn), tos);
}

uint8_t  getSn_TOS(uint8_t sn)
{
   return WIZCHIP_READ(Sn_TOS(sn));
}


void     setSn_TTL(uint8_t sn, uint8_t ttl)
{
   WIZCHIP_WRITE(Sn_TTL(sn), ttl);
}

uint8_t  getSn_TTL(uint8_t sn)
{
   return WIZCHIP_READ(Sn_TTL(sn));
}

void     setSn_RXBUF_SIZE(uint8_t sn, uint8_t rxbufsize)
{
   WIZCHIP_WRITE(Sn_RXBUF_SIZE(sn),rxbufsize);
}

uint8_t  getSn_RXBUF_SIZE(uint8_t sn)
{
   return WIZCHIP_READ(Sn_RXBUF_SIZE(sn));
}

void     setSn_TXBUF_SIZE(uint8_t sn, uint8_t txbufsize)
{
   WIZCHIP_WRITE(Sn_TXBUF_SIZE(sn), txbufsize);
}

uint8_t  getSn_TXBUF_SIZE(uint8_t sn)
{
   return WIZCHIP_READ(Sn_TXBUF_SIZE(sn));
}

//
uint16_t getSn_TX_FSR(uint8_t sn)
{
   uint16_t val=0,val1=0;
   do
   {
      val1 = WIZCHIP_READ(Sn_TX_FSR(sn));
      val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1));
      if (val1 != 0)
      {
        val = WIZCHIP_READ(Sn_TX_FSR(sn));
        val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn),1));
      }
   }while (val != val1);
   return val;
}

//
uint16_t getSn_TX_RD(uint8_t sn)
{
   uint16_t txrd;
   txrd = WIZCHIP_READ(Sn_TX_RD(sn));
   txrd = (txrd << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_RD(sn),1));
   return txrd;
}

void     setSn_TX_WR(uint8_t sn, uint16_t txwr)
{
   WIZCHIP_WRITE(Sn_TX_WR(sn),   (uint8_t)(txwr>>8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_TX_WR(sn),1), (uint8_t) txwr);
}

uint16_t getSn_TX_WR(uint8_t sn)
{
   uint16_t txwr;
   txwr = WIZCHIP_READ(Sn_TX_WR(sn));
   txwr = (txwr<<8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_WR(sn),1));
   return txwr;
}

//
uint16_t getSn_RX_RSR(uint8_t sn)
{
   uint16_t val=0,val1=0;
   do
   {
      val1 = WIZCHIP_READ(Sn_RX_RSR(sn));
      val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1));
      if (val1 != 0)
      {
        val = WIZCHIP_READ(Sn_RX_RSR(sn));
        val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn),1));
      }
   }while (val != val1);
   return val;
}

void     setSn_RX_RD(uint8_t sn, uint16_t rxrd)
{
   WIZCHIP_WRITE(Sn_RX_RD(sn),   (uint8_t)(rxrd>>8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_RX_RD(sn),1), (uint8_t) rxrd);
}

uint16_t getSn_RX_RD(uint8_t sn)
{
   uint16_t rxrd;
   rxrd = WIZCHIP_READ(Sn_RX_RD(sn));
   rxrd = (rxrd<<8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RD(sn),1));
   return rxrd;
}

//
uint16_t getSn_RX_WR(uint8_t sn)
{
   uint16_t rxwr;
   rxwr = WIZCHIP_READ(Sn_RX_WR(sn));
   rxwr = (rxwr<<8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_WR(sn),1));
   return rxwr;
}


void     setSn_FRAG(uint8_t sn, uint16_t frag)
{

   WIZCHIP_WRITE(Sn_FRAG(sn),  (uint8_t)(frag >>8));
   WIZCHIP_WRITE(WIZCHIP_OFFSET_INC(Sn_FRAG(sn),1), (uint8_t) frag);
}

uint16_t  getSn_FRAG(uint8_t sn)
{
   uint16_t frag;
   frag = WIZCHIP_READ(Sn_FRAG(sn));
   frag = (frag << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_FRAG(sn),1));
   return frag;
}

void     setSn_KPALVTR(uint8_t sn, uint8_t kpalvt)
{
   WIZCHIP_WRITE(Sn_KPALVTR(sn), kpalvt);

}

uint8_t  getSn_KPALVTR(uint8_t sn)
{
   return WIZCHIP_READ(Sn_KPALVTR(sn));
}
//////////////////////////////////////


/////////////////////////////////////
// Sn_TXBUF & Sn_RXBUF IO function //
/////////////////////////////////////
uint16_t getSn_RxMAX(uint8_t sn)
{
   return (getSn_RXBUF_SIZE(sn) << 10);
}

uint16_t getSn_TxMAX(uint8_t sn)
{
   return (getSn_TXBUF_SIZE(sn) << 10);
}

void wiz_send_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;
   if(len == 0)  return;
   ptr = getSn_TX_WR(sn);
   
   addrsel = (ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
   WIZCHIP_WRITE_BUF(addrsel,wizdata, len);
   
   ptr += len;
   setSn_TX_WR(sn,ptr);
}

void wiz_recv_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
   uint16_t ptr = 0;
   uint32_t addrsel = 0;
   
   if(len == 0) return;
   ptr = getSn_RX_RD(sn);
   addrsel = (ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
   
   WIZCHIP_READ_BUF(addrsel, wizdata, len);
   ptr += len;
   
   setSn_RX_RD(sn,ptr);
}


void wiz_recv_ignore(uint8_t sn, uint16_t len)
{
   uint16_t ptr = 0;
   ptr = getSn_RX_RD(sn);
   ptr += len;
   setSn_RX_RD(sn,ptr);
}

