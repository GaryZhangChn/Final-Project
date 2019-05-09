/**
 * @file		socket.h
 * @brief		Socket Driver Header File - Common
 * @version	1.1
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 *			2013/07/09 - 1.1 Network control part was devided
 * @author	modified by Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef	_SOCKET_H
#define	_SOCKET_H

//#include "common/common.h"
#define WINDOWFULL_MAX_RETRY_NUM	3
#define WINDOWFULL_WAIT_TIME		1000

//******  Doxygen Description is moved to doxygen document folder *****
#define SOCKSTAT_CLOSED			-1
#define SOCKSTAT_INIT			0
#define SOCKSTAT_LISTEN			1
#define SOCKSTAT_SYNSENT		2
#define SOCKSTAT_SYNRECV		3
#define SOCKSTAT_ESTABLISHED	4
#define SOCKSTAT_FIN_WAIT		5
#define SOCKSTAT_CLOSING		6
#define SOCKSTAT_TIME_WAIT		7
#define SOCKSTAT_CLOSE_WAIT		8
#define SOCKSTAT_LAST_ACK		9
#define SOCKSTAT_UDP			10

#define SOCKERR_BUSY			-1
#define SOCKERR_NOT_TCP			-2
#define SOCKERR_NOT_UDP			-3
#define SOCKERR_WRONG_ARG		-4
#define SOCKERR_WRONG_STATUS	-5
#define SOCKERR_CLOSED			-6
#define SOCKERR_CLOSE_WAIT		-7
#define SOCKERR_FIN_WAIT		-8
#define SOCKERR_NOT_ESTABLISHED	-9
#define SOCKERR_WINDOW_FULL		-10
#define SOCKERR_TIME_OUT		-11
#define SOCKERR_NULL_SRC_IP		-12
#define SOCKERR_BUF_NOT_ENOUGH	-13
#define SOCKERR_NOT_SPECIFIED	-14



int8_t  TCPServerOpen(uint8_t s, uint16_t port);
int8_t  TCPClientOpen(uint8_t s, uint16_t sport, uint8_t *dip, uint16_t dport);
int8_t  TCPCltOpenNB(uint8_t s, uint16_t sport, uint8_t *dip, uint16_t dport);
int8_t  TCPConnChk(uint8_t s);
int8_t  TCPDisconnect(uint8_t s);
int8_t  TCPClose(uint8_t s);
int8_t  TCPCloseNB(uint8_t s);
int8_t  TCPCloseCHK(uint8_t s);
int8_t  TCPClsRcvCHK(uint8_t s);
int32_t TCPSend(uint8_t s, const int8_t *buf, uint16_t len);
int8_t  TCPSendNB(uint8_t s, const int8_t *buf, uint16_t len);
int32_t TCPReSend(uint8_t s);
int8_t  TCPReSendNB(uint8_t s);
int32_t TCPSendCHK(uint8_t s);
int32_t TCPRecv(uint8_t s, int8_t *buf, uint16_t len);
int8_t  UDPOpen(uint8_t s, uint16_t port);
int8_t  UDPClose(uint8_t s);
int32_t UDPSend(uint8_t s, const int8_t *buf, uint16_t len, uint8_t *addr, uint16_t port);
int32_t UDPSendNB(uint8_t s, const int8_t *buf, uint16_t len, uint8_t *addr, uint16_t port);
int8_t  UDPSendCHK(uint8_t s);
int32_t UDPRecv(uint8_t s, int8_t *buf, uint16_t len, uint8_t *addr, uint16_t *port);



#endif //_SOCKET_H





