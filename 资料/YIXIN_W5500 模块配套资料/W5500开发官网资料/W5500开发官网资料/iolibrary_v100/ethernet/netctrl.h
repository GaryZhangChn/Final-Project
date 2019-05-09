/**
 * @file		netctrl.h
 * @brief		Network Control Driver Header File - Common
 * @version	1.0
 * @date		2013/07/09
 * @par Revision
 *			2013/07/09 - 1.0 Release (devided from socket)
 * @author	modified by Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

#ifndef	_NETCTRL_H
#define	_NETCTRL_H

//#include "common/common.h"


/**
 * DHCP mode value of @ref wiz_NetInfo.
 * 'dhcp' member variable of wiz_NetInfo struct can have one of these value
 */
typedef enum {	// 0 is not used (zero means just ignore dhcp config this time)
	NETINFO_STATIC = 1,	///< Indicate DHCP is disabled.
	NETINFO_DHCP,		///< Indicate DHCP is working.
} dhcp_mode;

/**
 * Indicate the member variable of @ref wiz_NetInfo.
 * This is used as a param of @ref ClsNetInfo function.
 */
typedef enum {
	//NI_MAC_ADDR,	//< Indicate MAC Address	// Uncomment if needed
	NI_IP_ADDR,		///< Indicate IP Address
	NI_SN_MASK,		///< Indicate Subnet Mask
	NI_GW_ADDR,		///< Indicate Gateway Address
	NI_DNS_ADDR		///< Indicate DNS Address
} netinfo_member;

/**
 * Common Network Information Structure.
 * This is used for everywhere related with network config
 */
typedef struct wiz_NetInfo_t
{
	uint8_t mac[6];		///< MAC Address variable
	uint8_t ip[4];		///< IPv4 Address variable
	uint8_t sn[4];		///< Subnet Mask variable
	uint8_t gw[4];		///< Gateway Address variable
	uint8_t dns[4];		///< DNS Server Address variable
	dhcp_mode dhcp;		///< DHCP mode variable (See:@ref dhcp_mode)
} wiz_NetInfo;
 

void   SetNetInfo(wiz_NetInfo *netinfo);
void   ClsNetInfo(netinfo_member member);
void   GetNetInfo(wiz_NetInfo *netinfo);
void   GetDstInfo(uint8_t s, uint8_t *dstip, uint16_t *dstport);
void   SetSocketOption(uint8_t option_type, uint16_t option_value);
int8_t   GetTCPSocketStatus(uint8_t s);
int8_t   GetUDPSocketStatus(uint8_t s);
uint16_t GetSocketTxFreeBufferSize(uint8_t s);
uint16_t GetSocketRxRecvBufferSize(uint8_t s);


#endif //_NETCTRL_H





