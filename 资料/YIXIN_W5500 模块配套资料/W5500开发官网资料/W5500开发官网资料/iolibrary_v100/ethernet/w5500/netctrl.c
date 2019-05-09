/**
 * @file		w5500/netctrl.c
 * @brief		Network Control Driver Source File - For w5500
 * @version	1.0
 * @date		2013/07/09
 * @par Revision
 *			2013/07/09 - 1.0 Release (devided from socket)
 * @author	modified by Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

//#define FILE_LOG_SILENCE
#include "common/common.h"



 
void SetNetInfo(wiz_NetInfo *netinfo)
{
	if(netinfo->mac[0] != 0x00 || netinfo->mac[1] != 0x00 || netinfo->mac[2] != 0x00 || 
		netinfo->mac[3] != 0x00 || netinfo->mac[4] != 0x00 || netinfo->mac[5] != 0x00)
		setSHAR(netinfo->mac);							// set local MAC address
	if(netinfo->ip[0] != 0x00 || netinfo->ip[1] != 0x00 || netinfo->ip[2] != 0x00 || 
		netinfo->ip[3] != 0x00) setSIPR(netinfo->ip);	// set local IP address
	if(netinfo->sn[0] != 0x00 || netinfo->sn[1] != 0x00 || netinfo->sn[2] != 0x00 || 
		netinfo->sn[3] != 0x00) setSUBR(netinfo->sn);	// set Subnet mask
	if(netinfo->gw[0] != 0x00 || netinfo->gw[1] != 0x00 || netinfo->gw[2] != 0x00 || 
		netinfo->gw[3] != 0x00) setGAR(netinfo->gw);	// set Gateway address
}

void ClsNetInfo(netinfo_member member)
{
	uint8_t zero[6] = {0,};

	DBGA("Reset Address(%d)", member);
	switch(member) {
	//case NI_MAC_ADDR:	// If need, uncomment
	//	setSHAR(zero);
	//	break;
	case NI_IP_ADDR:
		setSIPR(zero);
		break;
	case NI_SN_MASK:
		setSUBR(zero);
		break;
	case NI_GW_ADDR:
		setGAR(zero);
		break;
	default:
		ERRA("wrong member value (%d)", member);
	}
}
 
void GetNetInfo(wiz_NetInfo *netinfo)
{
	getSHAR(netinfo->mac); // get local MAC address
	getSIPR(netinfo->ip); // get local IP address
	getSUBR(netinfo->sn); // get subnet mask address
	getGAR(netinfo->gw); // get gateway address
}

void GetDstInfo(uint8_t s, uint8_t *dstip, uint16_t *dstport)
{
   uint16_t dport = 0;

   getSn_DIPR(s, dstip);
   dport = getSn_DPORT(s);
   *dstport = dport;

}
 
void SetSocketOption(uint8_t option_type, uint16_t option_value)
{
	switch(option_type){
	case 0:
		setRTR(option_value); // set retry duration for data transmission, connection, closing ...
		break;
	case 1:
		setRCR((uint8_t)(option_value&0x00FF)); // set retry count (above the value, assert timeout interrupt)
		break;
	case 2:
		setIMR((uint8_t)(option_value&0x00FF)); // set interrupt mask.
		break;
	default:
		break;
	}
}
 
int8_t GetTCPSocketStatus(uint8_t s)
{
	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	}

	switch(getSn_SR(s)){
	case SOCK_CLOSED: return SOCKSTAT_CLOSED;             // closed
	case SOCK_INIT: return SOCKSTAT_INIT;                 // init state
	case SOCK_LISTEN: return SOCKSTAT_LISTEN;             // listen state
	case SOCK_SYNSENT: return SOCKSTAT_SYNSENT;           // connection state
	case SOCK_SYNRECV: return SOCKSTAT_SYNRECV;           // connection state
	case SOCK_ESTABLISHED: return SOCKSTAT_ESTABLISHED;   // success to connect
	case SOCK_FIN_WAIT: return SOCKSTAT_FIN_WAIT;         // closing state
	case SOCK_CLOSING: return SOCKSTAT_CLOSING;           // closing state
	case SOCK_TIME_WAIT: return SOCKSTAT_TIME_WAIT;       // closing state
	case SOCK_CLOSE_WAIT: return SOCKSTAT_CLOSE_WAIT;     // closing state
	case SOCK_LAST_ACK: return SOCKSTAT_LAST_ACK;         // closing state
	default:
	   if((getMR()&0x0F) != Sn_MR_TCP)
			return SOCKERR_NOT_TCP;
		else return SOCKERR_WRONG_STATUS;
	}
}

int8_t GetUDPSocketStatus(uint8_t s)
{
	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_UDP;
	}

	switch(getSn_SR(s)){
	case SOCK_CLOSED: return SOCKSTAT_CLOSED; //  closed
	case SOCK_UDP: return SOCKSTAT_UDP;       //  udp socket
#if 0	
	case SOCK_MACRAW: return 12;	//  mac raw mode socket
	case SOCK_PPPOE: return 13;		//  pppoe socket
#endif
   default:
      if((getMR()&0x0F) != Sn_MR_UDP)
            return SOCKERR_NOT_UDP;
      else return SOCKERR_WRONG_STATUS;
	}
}

uint16_t GetSocketTxFreeBufferSize(uint8_t s)
{
	return getSn_TX_FSR(s); // get socket TX free buf size
}

uint16_t GetSocketRxRecvBufferSize(uint8_t s)
{
	return getSn_RX_RSR(s); // get socket RX recv buf size
}





