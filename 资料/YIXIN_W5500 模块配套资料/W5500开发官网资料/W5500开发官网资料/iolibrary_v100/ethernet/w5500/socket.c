/**
 * @file		w5500/socket.c
 * @brief		Socket Driver Source File - For w5500
 * @version	1.1
 * @date		2013/02/27
 * @par Revision
 *			2013/02/27 - 1.0 Release
 *			2013/07/09 - 1.1 Network control part was devided
 * @author	EunKyoung
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

//#define FILE_LOG_SILENCE
#include "common/common.h"


static uint16_t local_port = 0xC000;	// Dynamic Port: C000(49152) ~ FFFF(65535)
static uint16_t txrd_checker[_WIZCHIP_SOCK_NUM_];
static uint32_t tcp_resend_elapse[_WIZCHIP_SOCK_NUM_] = {0,};
uint8_t windowfull_retry_cnt[8];

uint8_t incr_windowfull_retry_cnt(uint8_t s);
void init_windowfull_retry_cnt(uint8_t s);

int8_t TCPServerOpen(uint8_t s, uint16_t port)
{
	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else DBG("start");

	if (port == 0) {	// if don't set the source port, set local_port number.
		if(local_port == 0xffff) local_port = 0xc000;
		else local_port++;
		port = local_port;
	}

	TCPClose(s);
    setSn_MR(s, Sn_MR_TCP);
	setSn_PORT(s, port );
	setSn_CR(s, Sn_CR_OPEN);  // run sockinit Sn_CR
	while(getSn_CR(s));        // wait to process the command...
        DBGA("Sn_SR = %.2x , Protocol = %.2x", getSn_SR(s), getSn_MR(s));
        if(getSn_SR(s) != SOCK_INIT) {
            DBGA("wrong status(%d)", getSn_SR(s)); 
		return SOCKERR_WRONG_STATUS;
	} else {
		    setSn_CR(s,Sn_CR_LISTEN);
  	        while(getSn_CR(s));        // wait to process the command...
	}
	return RET_OK;
}

int8_t TCPClientOpen(uint8_t s, uint16_t sport, uint8_t *dip, uint16_t dport)
{
	int8_t ret;

	DBG("start");
	ret = TCPCltOpenNB(s, sport, dip, dport);
	if(ret != RET_OK) return ret;

	do {
		ret = TCPConnChk(s);
	} while(ret == SOCKERR_BUSY);

	return ret;
}

int8_t TCPCltOpenNB(uint8_t s, uint16_t sport, uint8_t *dip, uint16_t dport)
{
	uint8_t srcip[4], snmask[4];

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else if(dip == NULL) {
		ERR("NULL Dst IP");
		return SOCKERR_WRONG_ARG;
	} else DBG("start");

	if (sport == 0) {	// if don't set the source port, set local_port number.
		if(local_port == 0xffff) local_port = 0xc000;
		else local_port++;
		sport = local_port;
	}

	TCPClose(s);
	setSn_MR(s, Sn_MR_TCP);
	setSn_PORT(s, sport);
	setSn_CR(s, Sn_CR_OPEN);  // run sockinit Sn_CR
	while(getSn_CR(s));        // wait to process the command...
	DBGA("Sn_SR = %.2x , Protocol = %.2x", getSn_SR(s), getSn_SR(s));
        
	getSIPR(srcip);
	getSUBR(snmask);
        
	if(	((dip[0] == 0xFF) && (dip[1] == 0xFF) && 
		 (dip[2] == 0xFF) && (dip[3] == 0xFF)) ||
	 	((dip[0] == 0x00) && (dip[1] == 0x00) && 
	 	 (dip[2] == 0x00) && (dip[3] == 0x00)) || (sport == 0x00) ) 
	{
		DBG("invalid ip or port");
		DBGA("SOCK(%d)-[%02x.%02x.%02x.%02x, %d]",s, 
			dip[0], dip[1], dip[2], dip[3] , sport);
		return SOCKERR_WRONG_ARG;
	}
	else if( (srcip[0]==0 && srcip[1]==0 && srcip[2]==0 && srcip[3]==0) && 
		(snmask[0]!=0 || snmask[1]!=0 || snmask[2]!=0 || snmask[3]!=0) ) //Mikej : ARP Errata
	{
		DBG("Source IP is NULL while SN Mask is Not NULL");
		return SOCKERR_NULL_SRC_IP;
	}
	else
	{
		setSn_DIPR(s,  dip);	// set destination IP
		setSn_DPORT(s, dport);
		//SetSubnet(sn);	// for ARP Errata
		setSn_CR(s, Sn_CR_CONNECT);
		while (getSn_CR(s));	// wait for completion
	}

	return RET_OK;
}

int8_t TCPConnChk(uint8_t s)
{
	uint8_t socksr;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	}
	
        socksr = getSn_SR(s);
	
	if(socksr == SOCK_ESTABLISHED) {
		//ClearSubnet();	// for ARP Errata
		return RET_OK;
     } else if(getSn_IR(s) & Sn_IR_TIMEOUT) {
		setSn_IR(s, Sn_IR_TIMEOUT);   // clear TIMEOUT Interrupt
	
		//ClearSubnet();	// for ARP Errata
		return SOCKERR_TIME_OUT;
	}

	return SOCKERR_BUSY;
}

int8_t TCPDisconnect(uint8_t s)
{
	ERR("Not implemented yet");
	return RET_NOK;
#if 0
	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else DBG("start");

    setSn_CR(s,Sn_CR_DISCON);
	while(getSn_CR(s));  // wait to process the command...
	return RET_OK;
#endif
}

int8_t TCPClose(uint8_t s)
{
	int8_t ret;

	DBG("start");
	ret = TCPCloseNB(s);
	if(ret != RET_OK) return ret;

	do {
		ret = TCPCloseCHK(s);
	} while(ret == SOCKERR_BUSY);

	return ret;
}

int8_t TCPCloseNB(uint8_t s)
{
	uint8_t status;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else DBG("start");
    setSn_CR(s,Sn_CR_DISCON);
	while(getSn_CR(s));  // wait to process the command...
	status = getSn_SR(s);
	if(status == SOCK_CLOSED) return SOCKERR_WRONG_STATUS;

	return RET_OK;
}

int8_t TCPCloseCHK(uint8_t s)
{
#define TIMEOUT_CLOSE_WAIT	200
	uint8_t status;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else DBG("start");

	status = getSn_SR(s);
	if(status == SOCK_CLOSED) goto END_OK;

	setSn_CR(s, Sn_CR_CLOSE);
	while(getSn_CR(s));  // wait to process the command...

END_OK:
    setSn_IR(s, 0xFF);	// interrupt all clear
	return RET_OK;
}

int8_t TCPClsRcvCHK(uint8_t s)
{
#define TIMEOUT_CLOSE_WAIT	200
	uint8_t status;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	}

	status = getSn_SR(s);
	if(status == SOCK_CLOSED) goto END_OK;
	if(status == SOCK_CLOSE_WAIT) {
		setSn_CR(s, Sn_CR_CLOSE);
		while(getSn_CR(s));  // wait to process the command...
	} else return SOCKERR_BUSY;

END_OK:
	setSn_IR(s,0xFF);	// interrupt all clear
	return RET_OK;
}

int32_t TCPSend(uint8_t s, const int8_t *buf, uint16_t len)
{
	int32_t ret;

	while(1) {
		ret = TCPSendNB(s, buf, len);
		if(ret == RET_OK) break;
		if(ret != SOCKERR_BUSY) return ret;
	}

	while(1) {
		ret = TCPSendCHK(s);
		if(ret >= 0 || ret != SOCKERR_BUSY) break;
	}
	
	return ret;
}

int8_t TCPSendNB(uint8_t s, const int8_t *buf, uint16_t len)
{
	uint8_t status = 0;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else if(len == 0) {
		ERR("Zero length");
		return SOCKERR_WRONG_ARG;
	} else DBG("start");

	status = getSn_SR(s);
	if(status == SOCK_CLOSED) return SOCKERR_CLOSED;
    if((getSn_MR(s) & 0x0F) != Sn_MR_TCP) return SOCKERR_NOT_TCP;

	if(status == SOCK_FIN_WAIT) return SOCKERR_FIN_WAIT;
	if(status != SOCK_ESTABLISHED && status != SOCK_CLOSE_WAIT) return SOCKERR_NOT_ESTABLISHED;

	init_windowfull_retry_cnt(s);
	if(len > getSn_TxMAX(s)) len = getSn_TxMAX(s); // check size not to exceed MAX size.
	if(GetSocketTxFreeBufferSize(s) < len) return SOCKERR_BUSY;

    wiz_send_data(s, (uint8_t*)buf, len);
    txrd_checker[s] = getSn_TX_RD(s);

	setSn_CR(s,Sn_CR_SEND);
	while (getSn_CR(s));	// wait to process the command...

	return RET_OK;
}

int32_t TCPReSend(uint8_t s)
{
	int32_t ret;

	while(1) {
		ret = TCPReSendNB(s);
		if(ret == RET_OK) break;
		if(ret != SOCKERR_BUSY) return ret;
	}

	while(1) {
		ret = TCPSendCHK(s);
		if(ret >= 0 || ret != SOCKERR_BUSY) break;
	}
	
	return ret;
}

int8_t TCPReSendNB(uint8_t s)
{
	uint8_t status=0;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else DBG("start");

	status = getSn_SR(s);
	if(status == SOCK_CLOSED) return SOCKERR_CLOSED;
	if((getSn_MR(s) & 0x0F) != Sn_MR_TCP) return SOCKERR_NOT_TCP;

	if(status == SOCK_FIN_WAIT) return SOCKERR_FIN_WAIT;
	if(status != SOCK_ESTABLISHED && status != SOCK_CLOSE_WAIT) return SOCKERR_NOT_ESTABLISHED;

	status = incr_windowfull_retry_cnt(s);
	if(status == 1) tcp_resend_elapse[s] = wizpf_get_systick();
	else if(status > WINDOWFULL_MAX_RETRY_NUM) return SOCKERR_WINDOW_FULL;
	else if(wizpf_tick_elapse(tcp_resend_elapse[s]) < WINDOWFULL_WAIT_TIME)
		return SOCKERR_BUSY;

	txrd_checker[s] = getSn_TX_RD(s);
    setSn_CR(s, Sn_CR_SEND);
	while (getSn_CR(s));	// wait to process the command...
	
	return RET_OK;
}

int32_t TCPSendCHK(uint8_t s)
{
	uint16_t txrd;

	if(!(getSn_IR(s) & Sn_IR_SENDOK)) {
		if(getSn_SR(s) == SOCK_CLOSED) {

                  DBG("SOCK_CLOSED");                    
			TCPClose(s);
			return SOCKERR_CLOSED;
		}
		return SOCKERR_BUSY;
	} else setSn_IR(s, Sn_IR_SENDOK);
	txrd = getSn_TX_RD(s);
	if(txrd > txrd_checker[s]) return txrd - txrd_checker[s];
	else return (0xffff - txrd_checker[s]) + txrd + 1;
}

int32_t TCPRecv(uint8_t s, int8_t *buf, uint16_t len)
{
	uint8_t status = 0;
	uint16_t RSR_len = 0;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_TCP;
	} else if(len == 0) {
		ERR("Zero length");
		return SOCKERR_WRONG_ARG;
	}

	RSR_len = GetSocketRxRecvBufferSize(s);	// Check Receive Buffer
	if(RSR_len == 0){
		status = getSn_SR(s);
		if(status == SOCK_CLOSED) return SOCKERR_CLOSED;
		if((getSn_MR(s) & 0x0F) != Sn_MR_TCP) return SOCKERR_NOT_TCP;
                
		if(status == SOCK_CLOSE_WAIT) return SOCKERR_CLOSE_WAIT;
		if(status != SOCK_ESTABLISHED && status != SOCK_CLOSE_WAIT) return SOCKERR_NOT_ESTABLISHED;
	} else {
		if(len < RSR_len) RSR_len = len;
		wiz_recv_data(s, (uint8_t*)buf, RSR_len);
		setSn_CR(s, Sn_CR_RECV);
		while(getSn_CR(s));		// wait to process the command...
	}

	return RSR_len;
}

int8_t UDPOpen(uint8_t s, uint16_t port)
{
	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_UDP;
	} else DBG("start");

	if (port == 0) {	// if don't set the source port, set local_port number.
		if(local_port == 0xffff) local_port = 0xc000;
		else local_port++;
		port = local_port;
	}

	UDPClose(s);
        
	setSn_MR(s, Sn_MR_UDP);
	setSn_PORT(s, port);
	setSn_CR(s, Sn_CR_OPEN);  // run sockinit Sn_CR
	while(getSn_CR(s));        // wait to process the command...
	DBGA("Sn_SR = %.2x , Protocol = %.2x", getSn_SR(s), getSn_MR(s));

        
	return RET_OK;
}

int8_t UDPClose(uint8_t s)
{
	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_UDP;
	} else DBG("start");

	setSn_CR(s, Sn_CR_CLOSE);
	while(getSn_CR(s));      // wait to process the command...
	setSn_IR(s, 0xFF);	// interrupt all clear

	return RET_OK;
}

int32_t UDPSend(uint8_t s, const int8_t *buf, uint16_t len, uint8_t *addr, uint16_t port)
{
	int32_t ret = 0;

	ret = UDPSendNB(s, buf, len, addr, port);
	if(ret < RET_OK) return ret;
	else len = ret;

	do {
		ret = UDPSendCHK(s);
		if(ret == RET_OK) return len;
		if(ret != SOCKERR_BUSY) return ret;
	} while(1);
}

int32_t UDPSendNB(uint8_t s, const int8_t *buf, uint16_t len, uint8_t *addr, uint16_t port)
{
	uint8_t srcip[4], snmask[4], status = 0;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_UDP;
	} else if(len == 0 || addr == NULL) {
		if(len == 0) ERR("Zero length");
		else ERR("NULL Dst IP");
		return SOCKERR_WRONG_ARG;
	} else DBG("start");

	status = getSn_SR(s);
	if(status == SOCK_CLOSED) return SOCKERR_CLOSED;
	if((getSn_MR(s) & 0x0F) != Sn_MR_UDP) return SOCKERR_NOT_UDP;        
	if(status != SOCK_UDP) return SOCKERR_NOT_UDP;

	if (len > getSn_TxMAX(s)) len = getSn_TxMAX(s); // check size not to exceed MAX size.

	getSIPR(srcip);
	getSUBR(snmask);

	if((addr[0]==0x00 && addr[1]==0x00 && addr[2]==0x00 && 
		addr[3]==0x00) || (port==0x00))
	{
		DBG("invalid ip or port");
		DBGA("SOCK(%d)-[%02x.%02x.%02x.%02x, %d, %d]",s, 
			addr[0], addr[1], addr[2], addr[3] , port, len);
		return SOCKERR_WRONG_ARG;
	}
	else if( (srcip[0]==0 && srcip[1]==0 && srcip[2]==0 && srcip[3]==0) && 
		(snmask[0]!=0 || snmask[1]!=0 || snmask[2]!=0 || snmask[3]!=0) ) //Mikej : ARP Errata
	{
		DBG("Source IP is NULL while SN Mask is Not NULL");
		return SOCKERR_NULL_SRC_IP;
	}
	else
	{
		setSn_DIPR(s, addr);
		setSn_DPORT(s, port);

		wiz_send_data(s, (uint8_t*)buf, len);
		//SetSubnet(sn);	// for ARP Errata

		setSn_CR(s, Sn_CR_SEND);
		while(getSn_CR(s));  // wait to process the command...
	}

	return len;
}

int8_t UDPSendCHK(uint8_t s)
{
	uint8_t ir = getSn_IR(s);

	
	if(!(ir & Sn_IR_SENDOK)) {
		if(ir & Sn_IR_TIMEOUT) {
			DBG("send fail");
			setSn_IR(s, (Sn_IR_SENDOK | Sn_IR_TIMEOUT)); // clear SEND_OK & TIMEOUT Interrupt
			return SOCKERR_TIME_OUT;
		}
		return SOCKERR_BUSY;
	} else setSn_IR(s,  Sn_IR_SENDOK);
	//ClearSubnet();	// for ARP Errata

	return RET_OK;
}

int32_t UDPRecv(uint8_t s, int8_t *buf, uint16_t len, uint8_t *addr, uint16_t *port)
{
	uint8_t prebuf[8], status = 0;
	uint16_t tmp_len = 0, RSR_len = 0;

	if(s > _WIZCHIP_SOCK_NUM_) {
		ERRA("wrong socket number(%d)", s);
		return SOCKERR_NOT_UDP;
	} else if(len == 0) {
		ERR("Zero length");
		return SOCKERR_WRONG_ARG;
	}

	status = getSn_SR(s);
	if(status == SOCK_CLOSED) return SOCKERR_CLOSED;
	if((getSn_MR(s) & 0x0F) != Sn_MR_UDP) return SOCKERR_NOT_UDP;
	if(status != SOCK_UDP) return SOCKERR_NOT_UDP;

	RSR_len = GetSocketRxRecvBufferSize(s);	// Check Receive Buffer of W5500
	if(RSR_len < 8) {
		DBGA("wrong data received (%d)", RSR_len);
		wiz_recv_ignore(s, RSR_len);
		setSn_CR(s, Sn_CR_RECV);
		while(getSn_CR(s));    
                
		return SOCKERR_NOT_SPECIFIED;
	} else {
		wiz_recv_data(s, prebuf, 8);
		setSn_CR(s, Sn_CR_RECV);	// 데이터를 처리한 후 이것을 해줘야 적용됨

		if(addr) {		// read peer's IP address, port number.
			addr[0] = prebuf[0];
			addr[1] = prebuf[1];
			addr[2] = prebuf[2];
			addr[3] = prebuf[3];
		}
		if(port) {
			*port = prebuf[4];
			*port = (*port << 8) + prebuf[5];
		}
		tmp_len = prebuf[6];
		tmp_len = (tmp_len << 8) + prebuf[7];
		while(getSn_CR(s));	// IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV); 명령 후 해야함, 시간 벌려고 바로 안함

		DBGA("UDP Recv - addr(%d.%d.%d.%d:%d), t(%d), R(%d)", 
			addr[0], addr[1], addr[2], addr[3], *port, tmp_len, RSR_len);
		if(tmp_len == 0) {
			ERR("UDP Recv len Zero - remove rest all");
			setSn_CR(s,Sn_CR_RECV);
			while(getSn_CR(s));
			return SOCKERR_NOT_SPECIFIED;
		}
		RSR_len = tmp_len;
	}

	if(len < RSR_len) {
		tmp_len = RSR_len - len;
		RSR_len = len;
		DBGA("Recv buffer not enough - len(%d)", len);
	} else tmp_len = 0;

	switch (getSn_MR(s) & 0x07)
	{
	case Sn_MR_UDP:
		wiz_recv_data(s, (uint8_t*)buf, RSR_len);
		setSn_CR(s, Sn_CR_RECV);

		if(tmp_len) {
			while(getSn_CR(s));
			DBG("Ignore rest data");			
			wiz_recv_ignore(s, tmp_len);
			setSn_CR(s, Sn_CR_RECV);
			while(getSn_CR(s));
			tmp_len = GetSocketRxRecvBufferSize(s);
			if(tmp_len) DBGA("another rest data(%d)", tmp_len);
			else DBG("No rest data");
		}
		break;
	//case Sn_MR_IPRAW:	
	case Sn_MR_MACRAW:
	default :
		break;
	}
	while(getSn_CR(s));
	
	return RSR_len;
}


uint8_t incr_windowfull_retry_cnt(uint8_t s)
{
	return ++windowfull_retry_cnt[s];
}

void init_windowfull_retry_cnt(uint8_t s)
{
	windowfull_retry_cnt[s] = 0;
}



