/**
 * @file		sockutil.h
 * @brief		Socket Utility Header File
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 * @author	modified by Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef _SOCKUTIL_H
#define _SOCKUTIL_H

//#include "common/common.h"

/**
 * @addtogroup sockwatch_module
 * @{
 * @def WATCH_SOCK_UDP_SEND
 * Indicate that 'UDP SEND' completion of this socket has to be watched.
 * @def WATCH_SOCK_TCP_SEND
 * Indicate that 'TCP SEND' completion of this socket has to be watched.
 * @def WATCH_SOCK_CONN_TRY
 * Indicate that 'CONNECT' completion of this socket has to be watched.
 * @def WATCH_SOCK_CONN_EVT
 * Indicate that 'CONNECT' event of this socket has to be watched.
 * @def WATCH_SOCK_CLS_TRY
 * Indicate that 'CLOSE' completion of this socket has to be watched.
 * @def WATCH_SOCK_CLS_EVT
 * Indicate that 'CLOSE' event of this socket has to be watched.
 * @def WATCH_SOCK_RECV
 * Indicate that 'RECEIVE' event of this socket has to be watched.
 * @def WATCH_SOCK_MASK_LOW
 * Mask all Completions of the socket.
 * @def WATCH_SOCK_MASK_HIGH
 * Mask all Events of the socket.
 * @def WATCH_SOCK_ALL_MASK
 * Mask all Completions and Events.
 * @typedef watch_cbfunc
 * Watch call back function form.
 * @}
 */
#define WATCH_SOCK_UDP_SEND		0x01
#define WATCH_SOCK_TCP_SEND 	0x02
#define WATCH_SOCK_CONN_TRY		0x04
#define WATCH_SOCK_CLS_TRY		0x08
#define WATCH_SOCK_CONN_EVT		0x10
#define WATCH_SOCK_CLS_EVT		0x20
#define WATCH_SOCK_RECV			0x40
#define WATCH_SOCK_MASK_LOW		0x0F
#define WATCH_SOCK_MASK_HIGH	0x70
#define WATCH_SOCK_ALL_MASK		0x7F

typedef void (*watch_cbfunc)(uint8_t id, uint8_t item, int32_t ret);

/**
 * @ingroup netdev_con_module
 * Call @ref wizchip_init after array param check
 */
#define WIZCHIP_INIT_WITH_MEMCHK(tx_size_v, rx_size_v) \
{ \
	uint8_t _i, *_tx, *_rx, _tx_cnt = 0, _rx_cnt = 0; \
	if(sizeof(tx_size_v)/sizeof(uint8_t) != _WIZCHIP_SOCK_NUM_ || \
		sizeof(rx_size_v)/sizeof(uint8_t) != _WIZCHIP_SOCK_NUM_) { \
		printf("Device Memory Configure fail 1"); \
		while(1); \
	} \
	_tx = (uint8_t*)tx_size_v; \
	_rx = (uint8_t*)rx_size_v; \
	for(_i=0; _i<_WIZCHIP_SOCK_NUM_; _i++) { \
		_tx_cnt += _tx[_i]; \
		_rx_cnt += _rx[_i]; \
	} \
	if(_tx_cnt > _WIZCHIP_SOCK_NUM_*2 || _rx_cnt > _WIZCHIP_SOCK_NUM_*2) { \
		printf("Device Memory Configure fail 2"); \
		while(1); \
	} \
	wizchip_init(tx_size_v, rx_size_v); \
}

int8_t sockwatch_open(uint8_t sock, watch_cbfunc cb);
int8_t sockwatch_close(uint8_t sock);
int8_t sockwatch_set(uint8_t sock, uint8_t item);
int8_t sockwatch_clr(uint8_t sock, uint8_t item);
int8_t sockwatch_chk(uint8_t sock, uint8_t item);
void sockwatch_run(void);
int8_t network_init(uint8_t dhcp_sock, void_func ip_update, void_func ip_conflict);
void network_disp(wiz_NetInfo *netinfo);
int8_t ip_check(int8_t *str, uint8_t *ip);
int8_t port_check(int8_t *str, uint16_t *port);
int8_t mac_check(int8_t *str, uint8_t *mac);
int8_t* inet_ntoa(uint32_t addr);
int8_t* inet_ntoa_pad(uint32_t addr);
uint32_t inet_addr(uint8_t* addr);
uint16_t htons( uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);	
uint32_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);

#endif	//_SOCKUTIL_H



