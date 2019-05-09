/**
 * @file		common.h
 * @brief		Library Common Header File.
 * This header file influence on all library area directly.
 *
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 * @author	Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef _COMMON_H
#define _COMMON_H


//-------------------------------------- Host Definition --------------------------------------
#include "xhw_types.h"
#include "xcore.h"
#include "xhw_memmap.h"
#include "xsysctl.h"
#include "xhw_spi.h"
#include "xgpio.h"
#include "xuart.h"
#include "xspi.h"

//-------------------------------------- Common Includes ----------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "common/util.h"
#include "ethernet/wizchip_conf.h"
#include "ethernet/netctrl.h"
#include "ethernet/socket.h"
#include "ethernet/sockutil.h"

#include "host/Nuvoton_M051/Cookie/cookie_evb.h"
#include "host/wizplatform.h"

#define RET_FAIL  1
#define RET_OK    0
#define RET_NOK      -1

#define VAL_HIGH  1
#define VAL_LOW      0

#define VAL_TOG      2
#define VAL_ON    1
#define VAL_OFF      0

#define VAL_SET      1
#define VAL_CLEAR 0

#define VAL_TRUE  1
#define VAL_FALSE 0

#define VAL_ENABLE   1
#define VAL_DISABLE  0

#define VAL_NONE  -1
#define VAL_INVALID  -2

//------------------------------------------- LOG ---------------------------------------------
#if !defined(WIZ_LOG_LEVEL) || (WIZ_LOG_LEVEL < 0) || (WIZ_LOG_LEVEL > 3)
#define WIZ_LOG_LEVEL 2
#endif

#if (WIZ_LOG_LEVEL > 0) && defined(PRINT_TIME_LOG) && !defined(FILE_LOG_SILENCE)
#define ERR(fmt)  do { printf("### ERROR ### [%5d.%03d] %s(%d): "fmt"\r\n", \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__); } while(0)
#define ERRA(fmt, ...)  do { printf("### ERROR ### [%5d.%03d] %s(%d): "fmt"\r\n", \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#define ERRF(fmt) do { printf("### ERROR ### [%5d.%03d] %s(%d): "fmt, \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__); } while(0)
#define ERRFA(fmt, ...) do { printf("### ERROR ### [%5d.%03d] %s(%d): "fmt, \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#elif (WIZ_LOG_LEVEL > 0) && !defined(PRINT_TIME_LOG) && !defined(FILE_LOG_SILENCE)
#define ERR(fmt)  do { printf("### ERROR ### %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__); } while(0)
#define ERRA(fmt, ...)  do { printf("### ERROR ### %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#define ERRF(fmt) do { printf("### ERROR ### %s(%d): "fmt, __FUNCTION__, __LINE__); } while(0)
#define ERRFA(fmt, ...) do { printf("### ERROR ### %s(%d): "fmt, __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#else
#define ERR(fmt)
#define ERRA(fmt, ...)
#define ERRF(fmt)
#define ERRFA(fmt, ...)
#endif

#if (WIZ_LOG_LEVEL > 1) && defined(PRINT_TIME_LOG) && !defined(FILE_LOG_SILENCE)
#define LOG(fmt)  do { printf("[%5d.%03d] "fmt"\r\n", \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000); } while(0)
#define LOGA(fmt, ...)  do { printf("[%5d.%03d] "fmt"\r\n", \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __VA_ARGS__); } while(0)
#define LOGF(fmt) do { printf("[%5d.%03d] "fmt, \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000); } while(0)
#define LOGFA(fmt, ...) do { printf("[%5d.%03d] "fmt, \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __VA_ARGS__); } while(0)
#elif (WIZ_LOG_LEVEL > 1) && !defined(PRINT_TIME_LOG) && !defined(FILE_LOG_SILENCE)
#define LOG(fmt)  do { printf(fmt"\r\n"); } while(0)
#define LOGA(fmt, ...)  do { printf(fmt"\r\n", __VA_ARGS__); } while(0)
#define LOGF(fmt) do { printf(fmt); } while(0)
#define LOGFA(fmt, ...) do { printf(fmt, __VA_ARGS__); } while(0)
#else
#define LOG(fmt)
#define LOGA(fmt, ...)
#define LOGF(fmt)
#define LOGFA(fmt, ...)
#endif

#if (WIZ_LOG_LEVEL > 2) && defined(PRINT_TIME_LOG) && !defined(FILE_LOG_SILENCE)
#define DBG(fmt)  do { printf("[D] [%5d.%03d] %s(%d): "fmt"\r\n", \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__); } while(0)
#define DBGA(fmt, ...)  do { printf("[D] [%5d.%03d] %s(%d): "fmt"\r\n", \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#define DBGF(fmt) do { printf("[D] [%5d.%03d] %s(%d): "fmt, \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__); } while(0)
#define DBGFA(fmt, ...) do { printf("[D] [%5d.%03d] %s(%d): "fmt, \
	wizpf_get_systick()/1000, wizpf_get_systick()%1000, __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#elif (WIZ_LOG_LEVEL > 2) && !defined(PRINT_TIME_LOG) && !defined(FILE_LOG_SILENCE)
#define DBG(fmt)  do { printf("[D] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__); } while(0)
#define DBGA(fmt, ...)  do { printf("[D] %s(%d): "fmt"\r\n", __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#define DBGF(fmt) do { printf("[D] %s(%d): "fmt, __FUNCTION__, __LINE__); } while(0)
#define DBGFA(fmt, ...) do { printf("[D] %s(%d): "fmt, __FUNCTION__, __LINE__, __VA_ARGS__); } while(0)
#else
#define DBG(fmt)
#define DBGA(fmt, ...)
#define DBGF(fmt)
#define DBGFA(fmt, ...)
#endif

#if (WIZ_LOG_LEVEL > 2) && !defined(FILE_LOG_SILENCE)
#define DBGCRTC(cond_v, fmt) do { if(cond_v) {ERR(fmt); while(1); } } while(0)
#define DBGCRTCA(cond_v, fmt, ...) do { if(cond_v) {ERRA(fmt, __VA_ARGS__); while(1); } } while(0)
#define DBGDUMP(data_p, len_v) print_dump(data_p, len_v)
#define DBGFUNC(func_p) func_p
#else
#define DBGCRTC(cond_v, fmt)
#define DBGCRTCA(cond_v, fmt, ...)
#define DBGDUMP(data_p, len_v)
#define DBGFUNC(func_p)
#endif

#if (WIZ_LOG_LEVEL > 0) && !defined(FILE_LOG_SILENCE)
#define NL1	printf("\r\n")
#define NL2	printf("\r\n\r\n")
#define NL3	printf("\r\n\r\n\r\n")
#else
#define NL1
#define NL2
#define NL3
#endif

//-------------------------------------------------------------------------------------------



#endif //_COMMON_H

