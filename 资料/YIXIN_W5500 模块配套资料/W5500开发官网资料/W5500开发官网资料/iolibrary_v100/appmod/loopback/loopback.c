/**
 * @file		loopback.c
 * @brief		Loopback Test Module Source File
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *		2013/02/22 - 1.0 Release
 * @author	
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

//#define FILE_LOG_SILENCE
#include "appmod/loopback/loopback.h"

#define TX_RX_MAX_BUF_SIZE	1024


int8_t data_buf[TX_RX_MAX_BUF_SIZE];


void loopback_tcps(uint8_t sock, uint16_t port)
{
	int32_t ret;
	int32_t SendLen, ReSendLen;

	ret = TCPRecv(sock, data_buf, TX_RX_MAX_BUF_SIZE);

	if(ret > 0){				// Received
		SendLen = TCPSend(sock, data_buf, ret);

		if(SendLen < ret){
			while(SendLen != ret){
				ReSendLen = TCPReSend(sock);

				if(ReSendLen > 0){
					SendLen += ReSendLen;

				} else if(ReSendLen == SOCKERR_WINDOW_FULL){
					LOG("Window Full");
					TCPClose(sock);
					DBG("TCP Socket Close");
					while(1);

				} else{
					break;
				}
			}
		}

	} else if(ret == SOCKERR_NOT_TCP){	// Not TCP Socket, It's UDP Socket
		DBG("UDP Socket Close");
		UDPClose(sock);
	} else if(ret == SOCKERR_CLOSED){		// Socket Closed
          LOGA("TCPS Loop-Back Started - sock(%d):port(%d)",(uint16_t)sock,port);
		TCPServerOpen(sock, port);
	}

	if(GetTCPSocketStatus(sock) == SOCKSTAT_CLOSE_WAIT){	// Close waiting
		TCPClose(sock);
	}
}
void loopback_tcpc(uint8_t sock, uint16_t port)
{							
	int32_t ret;
	int32_t SendLen, ReSendLen;
	uint8_t dip0[4] = {192, 168, 1, 200};
        uint16_t dport0 = 5002;
	
	ret = TCPRecv(sock, data_buf, TX_RX_MAX_BUF_SIZE);
	if(ret > 0){				// Received
		SendLen = TCPSend(sock, data_buf, ret);

		if(SendLen < ret){
			while(SendLen != ret){
				ReSendLen = TCPReSend(sock);

				if(ReSendLen > 0){
					SendLen += ReSendLen;

				} else if(ReSendLen == SOCKERR_WINDOW_FULL){
					LOG("Window Full");
					TCPClose(sock);
					DBG("TCP Socket Close");
					while(1);

				} else{
					break;
				}
			}
		}

	} else if(ret == SOCKERR_NOT_TCP){	// Not TCP Socket, It's UDP Socket
		DBG("UDP Socket Close");
		UDPClose(sock);
	} else if(ret == SOCKERR_CLOSED){		// Socket Closed
          LOGA("TCPC Loop-Back Started - sock(%d):port(%d)",(uint16_t)sock,port);
		TCPClientOpen(sock, port,(uint8_t*)dip0,dport0);
	}

	if(GetTCPSocketStatus(sock) == SOCKSTAT_CLOSE_WAIT){	// Close waiting
		TCPClose(sock);
	}
}
/**
 * Start UDP Loopback Test (Device Side).
 * For Loopback Test, need PC side Loopback test program 
 * @param sock Socket number to use
 * @param port Port number to use
 */
void loopback_udp(uint8_t sock, uint16_t port)
{
	int32_t ret;
	uint32_t destip = 0;
	uint16_t destport;

	ret = UDPRecv(sock, data_buf, TX_RX_MAX_BUF_SIZE, (uint8_t*)&destip, &destport);

	if(ret > 0){				// Received
		ret = UDPSend(sock, data_buf, ret, (uint8_t*)&destip ,destport);

		if(ret == SOCKERR_TIME_OUT){
			ERR("Timeout");
			UDPClose(sock);
			DBG("UDP Socket Close");
		}

	} else if(ret == SOCKERR_NOT_UDP){	// Not UDP Socket, It's TCP Socket
		DBG("TCP Socket Close");
		TCPClose(sock);
	} else if(ret == SOCKERR_CLOSED){		// Socket Closed
		LOGA("UDP Loop-Back Started - sock(%d):port(%d)",(uint16_t)sock, port);
		UDPOpen(sock, port);
	}
}



