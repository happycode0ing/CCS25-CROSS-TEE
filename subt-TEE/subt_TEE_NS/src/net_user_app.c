/*
 * net_user_app.c
 *
 *  Created on: 2024年9月14日
 *      Author: 27951
 */

#include "net_user_app.h"
//#include "encryption/encryption.h"
#include "SysTick/bsp_SysTick.h"

// added by lss
int dhcp_discover = 0;
int dhcp_request = 0;
//int pingreplyhook_num = 0;

/* Domain for the DNS Host lookup is used in this Example Project.
 * The project can be built with different *domain_name to validate the DNS client
 */
//char *domain_name = USR_TEST_DOMAIN_NAME;

/* IP address of the PC or any Device on the LAN/WAN where the Ping request is sent.
 * Note: Users needs to change this according to the LAN settings of your Test PC or device
 * when running this project.
 */
char *remote_ip_address = USR_TEST_PING_IP; //设置要Ping的IPv4地址

#if( ipconfigUSE_DHCP != 0 )
   /* DHCP populates these IP address, Sub net mask and Gateway Address. So start with this is zeroed out values
    * The MAC address is Test MAC address.
    */
    uint8_t ucMACAddress[ 6 ]       = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t ucIPAddress[ 4 ]        = {RESET_VALUE};
    uint8_t ucNetMask[ 4 ]          = {255, 255, 255, 128};
    uint8_t ucGatewayAddress[ 4 ]   = {132, 158, 124, 1};
    uint8_t ucDNSServerAddress[ 4 ] = {RESET_VALUE};
#else
    /* Static IP configuration, when DHCP mode is not used for the Example Project.
     * This needs to be populated by the user according to the Network Settings of your LAN.
     * This sample address taken from the LAN where it is tested. This is different for different LAN.
     * get the Address using the PC IPconfig details.
     */
    uint8_t ucMACAddress[ 6 ]       = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t ucIPAddress[ 4 ]        = {192, 168, 0, 52};
    uint8_t ucNetMask[ 4 ]          = {255, 255, 255, 0};
    uint8_t ucGatewayAddress[ 4 ]   = {192, 168, 0, 3};
    uint8_t ucDNSServerAddress[ 4 ] = {10, 60, 1, 2};
#endif


#if( ipconfigUSE_DHCP != 0 )
    NetworkAddressingParameters_t xNetworkAddressing;   // modified by lss: 去掉了extern关键字
    NetworkAddressingParameters_t xNd = {RESET_VALUE, RESET_VALUE, RESET_VALUE, RESET_VALUE, RESET_VALUE};
#endif

uint32_t  dhcp_in_use   = RESET_VALUE;
ping_data_t ping_data   = {RESET_VALUE, RESET_VALUE, RESET_VALUE};



/*******************************************************************************************************************//**
 * @brief      Generates 32 bit Random number
 * @param[in]  void
 * @retval     Random Number
 **********************************************************************************************************************/
uint32_t ulRand()
{
    /* example of a 32-bit random number generator.
     * Here rand() returns a 15-bit number. so create 32 bit Random number using 15 bit rand()
     */
    uint32_t ulResult =
        ((((uint32_t) rand()) & 0x7fffuL)       ) |
        ((((uint32_t) rand()) & 0x7fffuL) << 15 ) |
        ((((uint32_t) rand()) & 0x0003uL) << 30 );
    return ulResult;
}

 /*******************************************************************************************************************//**
 * @brief      Generates 32 sequence number
 * @param[in]  Source IP, Source Port, Destination IP and Destination Port.
 * @retval     Sequence Number
 **********************************************************************************************************************/
uint32_t ulApplicationGetNextSequenceNumber(uint32_t ulSourceAddress, uint16_t usSourcePort, uint32_t ulDestinationAddress,
                                            uint16_t usDestinationPort)
{
    /* Here we need to get random number for the sequence number.
     * This is just for testing purpose, so software rand() is okay.
     * This can also be tied to the TRNG.
     */

     return((ulSourceAddress + ulDestinationAddress + usSourcePort + usDestinationPort) && ulRand());
}

/*******************************************************************************************************************//**
* @brief      Send ICMP Ping request  based on the user input IP Address.
* @param[in]  IP address to Ping
* @retval     Sequence Number
**********************************************************************************************************************/
BaseType_t vSendPing( const char *pcIPAddress)
{
    uint32_t ulIPAddress = RESET_VALUE;

    /*
     * The pcIPAddress parameter holds the destination IP address as a string in
     * decimal dot notation (for example, ��192.168.0.200��). Convert the string into
     * the required 32-bit format.
     */
    ulIPAddress = FreeRTOS_inet_addr(pcIPAddress);

    /*
     * Send a ping request containing 8 data bytes.  Wait (in the Blocked state) a
     * maximum of 100ms for a network buffer into which the generated ping request
     * can be written and sent.
     */
    return(FreeRTOS_SendPingRequest(ulIPAddress, 8, 100 / portTICK_PERIOD_MS));
}

/*******************************************************************************************************************//**
* @brief      User Hook for the Ping Reply. vApplicationPingReplyHook() is called by the TCP/IP
*             stack when the stack receives a ping reply.
* @param[in]  Ping reply status and Identifier
* @retval     None
**********************************************************************************************************************/
void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
    (void)  usIdentifier;

    // added by lss
//    pingreplyhook_num++;

    switch( eStatus )
    {
        /* A valid ping reply has been received */
        case eSuccess    :
            ping_data.received++;
            break;
            /* A reply was received but it was not valid. */
        case eInvalidData :
        default:
            ping_data.lost++;
            break;
    }
}


/*******************************************************************************************************************//**
* @brief      This is the User Hook for the DHCP Response. xApplicationDHCPHook() is called by DHCP Client Code when DHCP
*             handshake messages are exchanged from the Server.
* @param[in]  Different Phases of DHCP Phases and the Offered IP Address
* @retval     Returns DHCP Answers.
**********************************************************************************************************************/
#if( ipconfigUSE_DHCP != 0 )
eDHCPCallbackAnswer_t xApplicationDHCPHook( eDHCPCallbackPhase_t eDHCPPhase,
                                            uint32_t ulIPAddress )
{
    eDHCPCallbackAnswer_t eReturn = eDHCPContinue;
  /*
   * This hook is called in a couple of places during the DHCP process, as identified by the eDHCPPhase parameter.
   */
  switch( eDHCPPhase )
  {
    case eDHCPPhasePreDiscover  :
      /*
       *  A DHCP discovery is about to be sent out.  eDHCPContinue is returned to allow the discovery to go out.
       *  If eDHCPUseDefaults had been returned instead then the DHCP process would be stopped and the statically
       *  configured IP address would be used.
       *  If eDHCPStopNoChanges had been returned instead then the DHCP process would be stopped and whatever the
       *  current network configuration was would continue to be used.
       */
        dhcp_discover = 1;
        break;

    case eDHCPPhasePreRequest  :
      /* An offer has been received from the DHCP server, and the offered IP address is passed in the ulIPAddress
       * parameter.
       */

        /*
         * The sub-domains don��t match, so continue with the DHCP process so the offered IP address is used.
         */
        /* Update the Structure, the DHCP state Machine is not updating this */
        dhcp_request = 1;
        xNetworkAddressing.ulDefaultIPAddress = ulIPAddress;
        xNetworkAddressing.ulNetMask = FreeRTOS_GetNetmask(); // 255.255.255.0
        xNetworkAddressing.ulGatewayAddress = FreeRTOS_GetGatewayAddress(); // 192.168.3.1
        xNetworkAddressing.ulDNSServerAddress = FreeRTOS_GetDNSServerAddress(); // 192.168.3.1
        dhcp_in_use = 1;
      break;

    default :
      /*
       * Cannot be reached, but set eReturn to prevent compiler warnings where compilers are disposed to generating one.
       */
        break;
  }

  return eReturn;
}
#endif


/*******************************************************************************************************************//**
 * @brief      Prints the Ping response on to the RTT console
 * @param[in]  void
 * @retval     None
 **********************************************************************************************************************/
void print_pingResult(void)
{
    printf("\r\n \r\nPing Statistics for %s :\r\n",(char *)remote_ip_address);
//    printf("pingreplyhook_num = %d\r\n", pingreplyhook_num);
    printf("\r\nPackets: Sent  = %02ld, Received = %02ld, Lost = %02ld \r\n",ping_data.sent,ping_data.received,ping_data.lost);
}

/*******************************************************************************************************************//**
 * @brief      Creates and prints the the IP configuration to display
 * @param[in]  void
 * @retval     None
 **********************************************************************************************************************/
void print_ipconfig(void)
{
#if( ipconfigUSE_DHCP != 0 )

//    printf("ipconfigUSE_DHCP != 0\r\n");
//    if(dhcp_discover)
//    {
//        printf("A DHCP discovery is sent out\r\n");
//    }
//    if(dhcp_request)
//    {
//        printf("An offer has been received from the DHCP server\r\n");
//    }


    if(dhcp_in_use)
    {
//        printf("dhcp_in_use = 1\r\n");
        ucNetMask[3] = (uint8_t)((xNd.ulNetMask & 0xFF000000) >> 24);
        ucNetMask[2] = (uint8_t)((xNd.ulNetMask & 0x00FF0000) >> 16);
        ucNetMask[1] = (uint8_t)((xNd.ulNetMask & 0x0000FF00) >> 8);
        ucNetMask[0] = (uint8_t)(xNd.ulNetMask & 0x000000FF);

        ucGatewayAddress[3] = (uint8_t)((xNd.ulGatewayAddress & 0xFF000000)>> 24);;
        ucGatewayAddress[2] = (uint8_t)((xNd.ulGatewayAddress & 0x00FF0000)>> 16);
        ucGatewayAddress[1] = (uint8_t)((xNd.ulGatewayAddress & 0x0000FF00)>> 8);
        ucGatewayAddress[0] = (uint8_t)(xNd.ulGatewayAddress & 0x000000FF);

        ucDNSServerAddress[3] = (uint8_t)((xNd.ulDNSServerAddress & 0xFF000000)>> 24);
        ucDNSServerAddress[2] = (uint8_t)((xNd.ulDNSServerAddress & 0x00FF0000)>> 16);
        ucDNSServerAddress[1] = (uint8_t)((xNd.ulDNSServerAddress & 0x0000FF00)>> 8);
        ucDNSServerAddress[0] = (uint8_t)(xNd.ulDNSServerAddress & 0x000000FF);

        ucIPAddress[3] = (uint8_t)((xNd.ulDefaultIPAddress & 0xFF000000) >> 24);
        ucIPAddress[2] = (uint8_t)((xNd.ulDefaultIPAddress & 0x00FF0000) >> 16);
        ucIPAddress[1] = (uint8_t)((xNd.ulDefaultIPAddress & 0x0000FF00) >> 8);
        ucIPAddress[0] = (uint8_t)(xNd.ulDefaultIPAddress & 0x000000FF);
    }
#endif
    printf("\r\nEthernet adapter for 野火启明6M5:\r\n");

    printf("\tDescription . . . . . . . . . . . : RA6M5 Ethernet\r\n");
//    printf("\tPhysical Address. . . . . . . . . : %02x-%02x-%02x-%02x-%02x-%02x\r\n",
//            ucMACAddress[0],ucMACAddress[1],ucMACAddress[2],ucMACAddress[3],ucMACAddress[4],ucMACAddress[5]);
    printf("\tDHCP Enabled. . . . . . . . . . . : %s\r\n",dhcp_in_use?"Yes":"No");
    printf("\tIPv4 Address. . . . . . . . . . . : %d.%d.%d.%d\r\n",ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
    printf("\tSubnet Mask . . . . . . . . . . . : %d.%d.%d.%d\r\n",ucNetMask[0],ucNetMask[1],ucNetMask[2],ucNetMask[3]);
    printf("\tDefault Gateway . . . . . . . . . : %d.%d.%d.%d\r\n",ucGatewayAddress[0],ucGatewayAddress[1],ucGatewayAddress[2],ucGatewayAddress[3]);
    printf("\tDNS Servers . . . . . . . . . . . : %d.%d.%d.%d\r\n",ucDNSServerAddress[0],ucDNSServerAddress[1],ucDNSServerAddress[2],ucDNSServerAddress[3]);
}


/*******************************************************************************************************************//**
 * @brief      DNS Query for the requested Domain name.  Uses the FreeRTOS Client API  FreeRTOS_gethostbyname
 *             to get the IP address for the domain name
 * @param[in]  Domain name
 * @retval     None
 **********************************************************************************************************************/
void dnsQuerryFunc(char *domain)
{
    uint32_t ulIPAddress = RESET_VALUE;
    int8_t cBuffer[ 16 ] = {RESET_VALUE};

    /* Lookup the IP address of the FreeRTOS.org website. */
    ulIPAddress = FreeRTOS_gethostbyname((char*)domain);

    if( ulIPAddress != 0 )
    {
        /* Convert the IP address to a string. */
        FreeRTOS_inet_ntoa( ulIPAddress, ( char * ) cBuffer);

        /* Print out the IP address obtained from the DNS lookup. */
        printf ("\r\nDNS Lookup for \"www.freertos.org\" is      : %s  \r\n", cBuffer);
    }
    else
    {
        printf ("\r\nDNS Lookup failed for \"www.freertos.org\" \r\n");
    }
}

/*******************************************************************************************************************//**
 * @brief      This Function checks the Network status (Both Ethernet and IP Layer). If the Network is down
 *             the Application will not send any data on the network.
 * @param[in]  None
 * @retval     Network Status
 **********************************************************************************************************************/
uint32_t isNetworkUp(void)
{
    fsp_err_t  eth_link_status = FSP_ERR_NOT_OPEN;
    BaseType_t networkUp = pdFALSE;
    uint32_t network_status = (IP_LINK_UP | ETHERNET_LINK_UP);

    networkUp = FreeRTOS_IsNetworkUp();
    eth_link_status = R_ETHER_LinkProcess(g_ether0.p_ctrl);

    if((FSP_SUCCESS == eth_link_status) && (pdTRUE == networkUp))
    {
        return network_status;
    }
    else
    {
        if(FSP_SUCCESS != eth_link_status)
        {
            network_status |= ETHERNET_LINK_DOWN;
        }
        else if(FSP_SUCCESS == eth_link_status)
        {
            network_status |= ETHERNET_LINK_UP;
        }

        if(pdTRUE != networkUp)
        {
             network_status |= IP_LINK_DOWN;
        }
        else if(pdTRUE == networkUp)
        {
             network_status |= IP_LINK_UP;
        }
        return network_status;
    }
}


/*******************************************************************************************************************//**
 * @brief      Update the DHCP info to the User data structure.
 * @param[in]  None
 * @retval     None
 **********************************************************************************************************************/
#if( ipconfigUSE_DHCP != 0 )
void updateDhcpResponseToUsr(void)
{
    if(dhcp_in_use)
    {
        memcpy(&xNd, &xNetworkAddressing, sizeof(xNd));
    }
}
#endif

/*******************************************************************************************************************//**
 * @brief      DHCP Hook function to populate the user defined Host name for the Kit.
 * @param[in]  None
 * @retval     Hostname
 **********************************************************************************************************************/
#if( ipconfigDHCP_REGISTER_HOSTNAME == 1 )
const char *pcApplicationHostnameHook(void)
{
    return "启明6M5-RA6M5";
}
#endif

int vTCPSend(const char *pcIPAddress, char *pcBufferToTransmit, const size_t xTotalLengthToSend )
{
    int ret = -1;
    Socket_t xSocket;
    struct freertos_sockaddr xRemoteAddress;
    BaseType_t xAlreadyTransmitted = 0, xBytesSent = 0;
    TaskHandle_t xRxTask = NULL;
    size_t xLenToSend;

    /* Set the IP address (192.168.0.200) and port (1500) of the remote socket
       to which this client socket will transmit. */
    memset( &xRemoteAddress, 0, sizeof(xRemoteAddress) );
    xRemoteAddress.sin_port = FreeRTOS_htons(5040);
    xRemoteAddress.sin_address.ulIP_IPv4 = FreeRTOS_inet_addr(pcIPAddress);
    xRemoteAddress.sin_family = FREERTOS_AF_INET4;

    /* Create a socket. */
    xSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                               FREERTOS_SOCK_STREAM,/* FREERTOS_SOCK_STREAM for TCP. */
                               FREERTOS_IPPROTO_TCP );
    configASSERT( xSocket != FREERTOS_INVALID_SOCKET );
    printf("Socket created\r\n");

    if(FreeRTOS_bind(xSocket, NULL, sizeof(struct freertos_sockaddr)))
    {
        printf("Socket bind Failed!\r\n");
        return ret;
    }
    printf("Socket bind Success!\r\n");

    /* Connect to the remote socket. The socket has not previously been bound to
       a local port number so will get automatically bound to a local port inside
       the FreeRTOS_connect() function. */
    BaseType_t conret = FreeRTOS_connect( xSocket, &xRemoteAddress, sizeof( xRemoteAddress ) );
    printf("connection return: %ld\r\n", conret);
    if(conret == -pdFREERTOS_ERRNO_ETIMEDOUT)
    {
        printf("Connection timed out!\r\n");
    }
    else if(conret == -pdFREERTOS_ERRNO_EINVAL)
    {
        printf("xSocket is not a valid TCP socket!\r\n");
    }
    else if(conret == -pdFREERTOS_ERRNO_EALREADY)
    {
        printf("xSocket was already connected!\r\n");
    }
    else if(conret == -pdFREERTOS_ERRNO_EINPROGRESS || conret == -pdFREERTOS_ERRNO_EAGAIN)
    {
        printf("xSocket is not in a state that allows a connect operation!\r\n");
    }
    else if(conret == -pdFREERTOS_ERRNO_EWOULDBLOCK)
    {
        printf("socket has a read block time of zero and the connect operation cannot succeed immediately!\r\n");
    }

    if( conret == 0 )
    {
        printf("Socket connected\r\n");
        /* Keep sending until the entire buffer has been sent. */
        while( xAlreadyTransmitted < xTotalLengthToSend )
        {
            /* How many bytes are left to send? */
            xLenToSend = xTotalLengthToSend - xAlreadyTransmitted;
            xBytesSent = FreeRTOS_send( /* The socket being sent to. */
                                        xSocket,
                                        /* The data being sent. */
                                        &( pcBufferToTransmit[ xAlreadyTransmitted ] ),
                                        /* The remaining length of data to send. */
                                        xLenToSend,
                                        /* ulFlags. */
                                        0 );

            if( xBytesSent >= 0 )
            {
                /* Data was sent successfully. */
                xAlreadyTransmitted += xBytesSent;
            }
            else
            {
                /* Error - break out of the loop for graceful socket close. */
                break;
            }
        }
        printf("TCP messages send finished\r\n");
        ret = 0;
    }

    /* Initiate graceful shutdown. */
    FreeRTOS_shutdown( xSocket, FREERTOS_SHUT_RDWR );

    /* Wait for the socket to disconnect gracefully (indicated by FreeRTOS\_recv()
       returning a -pdFREERTOS_ERRNO_EINVAL error) before closing the socket. */
    while( FreeRTOS_recv( xSocket, pcBufferToTransmit, xTotalLengthToSend, 0 ) >= 0 )
    {
        /* Wait for shutdown to complete. If a receive block time is used then
           this delay will not be necessary as FreeRTOS_recv() will place the RTOS task
           into the Blocked state anyway. */
        vTaskDelay(250);

        /* Note - real applications should implement a timeout here, not just
           loop forever. */
    }
    printf("Wait for the socket to disconnect gracefully\r\n");

    /* The socket has shut down and is safe to close. */
    FreeRTOS_closesocket( xSocket );
    return ret;
}

int vUDPSend(const char *pcIPAddress, char *pcBufferToTransmit, const size_t xTotalLengthToSend)
{
    int ret = -1;
    Socket_t xSocket;
    struct freertos_sockaddr xDestinationAddress;
    uint8_t cString[50];
    uint32_t ulCount = 0UL;
    const TickType_t x1000ms = 1000UL / portTICK_PERIOD_MS;

    /* Send strings to port 5050 on IP address 192.168.3.5 */
    memset( &xDestinationAddress, 0, sizeof(xDestinationAddress) );
    xDestinationAddress.sin_address.ulIP_IPv4 = FreeRTOS_inet_addr(pcIPAddress);
    xDestinationAddress.sin_family = FREERTOS_AF_INET4;
    xDestinationAddress.sin_port = FreeRTOS_htons(UDP_TEST_PORT);

    /* Create the socket. */
    xSocket = FreeRTOS_socket( FREERTOS_AF_INET, /* Used for IPv4 UDP socket. */
                                  /* FREERTOS_AF_INET6 can be used for IPv6 UDP socket. */
                               FREERTOS_SOCK_DGRAM,/*FREERTOS_SOCK_DGRAM for UDP.*/
                               FREERTOS_IPPROTO_UDP );

    /* Check the socket was created. */
    configASSERT( xSocket != FREERTOS_INVALID_SOCKET );

    FreeRTOS_bind( xSocket, &xDestinationAddress, sizeof(xDestinationAddress) );

    /* NOTE: FreeRTOS_bind() is not called. This will only work if
       ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND is set to 1 in FreeRTOSIPConfig.h. */

    uint64_t start, end;
    uint64_t cpu_time_used1, cpu_time_used2;

    for( ; ulCount < 1; ulCount++)
    {
        uint64_t nums = 1;  // data length = 1024B * nums, nums KB
        int xret;

        /* Create the string that is sent. */
        sprintf( cString, "%s", pcBufferToTransmit);

        /* Send the string to the UDP socket. ulFlags is set to 0, so the standard
           semantics are used. That means the data from cString[] is copied
           into a network buffer inside FreeRTOS_sendto(), and cString[] can be
           reused as soon as FreeRTOS_sendto() has returned. */

        start = system_get_time();

//        for(int i = 0; i < nums; i++)
//        {
            xret = FreeRTOS_sendto( xSocket, cString, xTotalLengthToSend, 0, &xDestinationAddress, sizeof( xDestinationAddress ) );
//        }

        end = system_get_time();
        cpu_time_used2 = end - start;

        if(xret > 0)
        {
            printf("send(%u):\r\n", xTotalLengthToSend);
            for(unsigned int i = 0; i < xTotalLengthToSend; i++)
            {
                printf("%02x", cString[i]);
            }
            printf("\r\n");
            printf("%u\r\n", (unsigned int)((cpu_time_used2) & 0xffffffff));
            ret = 0;
        }
        else{
            printf("send failed\r\n");
        }

        /* Wait until it is time to send again. */
        vTaskDelay( x1000ms );
    }

//    /* 统计内存开销 */
//    // 获取当前空闲内存大小
//    size_t free_heap0 = xPortGetFreeHeapSize();
//
//    // 获取最小剩余内存大小
//    size_t min_free_heap0 = xPortGetMinimumEverFreeHeapSize();
//
//    // 获取堆的总大小
//    size_t total_heap0 = configTOTAL_HEAP_SIZE;
//
//    // 计算已分配内存大小
//    size_t used_heap0 = total_heap0 - free_heap0;
//
//    // 计算最大内存使用量
//    size_t max_used_heap0 = total_heap0 - min_free_heap0;
//
//    printf("Used heap size: %dB\n", used_heap0);
//    printf("Maximum used heap size: %dB\n", max_used_heap0);

    return ret;
}
