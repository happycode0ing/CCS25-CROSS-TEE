/*
 * net_user_app.h
 *
 *  Created on: 2024年9月14日
 *      Author: 27951
 */

#ifndef __NET_USER_APP_H
#define __NET_USER_APP_H


#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_Sockets.h"
#include <stdlib.h>
#include "uart/nsc_bsp_uart.h"

#if( ipconfigUSE_DHCP != 0 )
    #include "FreeRTOS_DHCP.h"
#endif


#define RESET_VALUE             (0x00)

/* User APP Data */

//#define USR_TEST_DOMAIN_NAME "www.freertos.org"
#define UDP_TEST_PORT 5050
#define USR_TEST_PING_IP "192.168.3.5"
#define USR_PING_COUNT 100

#define SUCCESS 0
#define PRINT_UP_MSG_DISABLE 0x01
#define PRINT_DOWN_MSG_DISABLE 0x02
#define PRINT_NWK_USR_MSG_DISABLE 0x04


#define ETHERNET_LINK_DOWN 0x01
#define ETHERNET_LINK_UP 0x00
#define IP_LINK_DOWN 0x02
#define IP_LINK_UP 0x00


#if( ipconfigUSE_DHCP != 0 )
    void updateDhcpResponseToUsr(void);
#endif

#if( ipconfigUSE_DHCP != 0 )
    eDHCPCallbackAnswer_t xApplicationDHCPHook( eDHCPCallbackPhase_t eDHCPPhase, uint32_t ulIPAddress );
#endif

#if( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
    /* DHCP has an option for clients to register their hostname.  It doesn't
    have much use, except that a device can be found in a router along with its
    name. If this option is used the callback below must be provided by the
    application writer to return a const string, denoting the device's name. */
    const char *pcApplicationHostnameHook( void );
#endif /* ipconfigDHCP_REGISTER_HOSTNAME */


uint32_t ulRand();
uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                             uint16_t usSourcePort,
                                             uint32_t ulDestinationAddress,
                                             uint16_t usDestinationPort );
uint32_t isNetworkUp(void);
BaseType_t vSendPing( const char *pcIPAddress);
void print_ipconfig(void);
void print_pingResult(void);
void dnsQuerryFunc(char *domain_name);
int vTCPSend(const char *pcIPAddress, char *pcBufferToTransmit, const size_t xTotalLengthToSend);
int vUDPSend(const char *pcIPAddress, char *pcBufferToTransmit, const size_t xTotalLengthToSend);


typedef struct st_ping_data
{
    uint32_t sent;     // Ping Request
    uint32_t received; // Ping Response
    uint32_t lost;     // Ping failure
} ping_data_t;


#endif
