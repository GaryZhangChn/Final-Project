/**
 * @file		loopback.h
 * @brief		Loopback Test Module Header File
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 * @author	
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef _LOOPBACK_H
#define _LOOPBACK_H

#include "common/common.h"

/**
 * Start TCP Loopback Test (Device Side).
 * For Loopback Test, need PC side Loopback test program
 * @param sock Socket number to use
 * @param port Port number to use
 */
void loopback_tcps(uint8_t sock, uint16_t port);
void loopback_tcpc(uint8_t sock, uint16_t port);
void loopback_udp(uint8_t sock, uint16_t port);

#endif //_LOOPBACK_H



