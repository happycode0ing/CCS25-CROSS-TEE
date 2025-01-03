//#include "net_thread.h"
//                /* Net Thread entry function */
//                /* pvParameters contains TaskHandle_t */
//                void net_thread_entry(void * pvParameters)
//                {
//                    FSP_PARAMETER_NOT_USED(pvParameters);

//                    /* TODO: add your own code here */
//                    while(1)
//                    {
//                        vTaskDelay(1);
//                    }
//                }


#include "net_thread.h"
#include "net_user_app.h"
#include "my_nsc_api.h"
#include "uart/nsc_bsp_uart.h"
#include "canfd/nsc_bsp_canfd0.h"
#include "canfd/nsc_bsp_canfd1.h"
#include "encryption/encryption.h"
#include "SysTick/bsp_SysTick.h"
#include <stdint.h>

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

//extern char *domain_name;
extern char *remote_ip_address;

extern uint8_t ucMACAddress[ 6 ];
extern uint8_t ucIPAddress[ 4 ];
extern uint8_t ucNetMask[ 4 ];
extern uint8_t ucGatewayAddress[ 4 ];
extern uint8_t ucDNSServerAddress[ 4 ];

extern ping_data_t ping_data;
uint32_t  usrPingCount  = RESET_VALUE;
static uint32_t usr_print_ability = RESET_VALUE;

/* 外部变量和函数声明 */
extern volatile bool canfd0_rx_complete_flag;
extern can_frame_t canfd0_tx_frame;
extern can_frame_t canfd0_rx_frame;

extern volatile bool canfd1_rx_complete_flag;
extern can_frame_t canfd1_rx_frame;
extern can_frame_t canfd1_tx_frame;

extern volatile bool canfd0_senddata_enable;
extern volatile bool canfd1_senddata_enable;


/* Net Thread entry function */
/* pvParameters contains TaskHandle_t */
void net_thread_entry(void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    BaseType_t status = pdFALSE;
    fsp_pack_version_t version = {RESET_VALUE};


    /* 调试串口初始化 */
    Debug_UART4_Init(); // SCI4 UART 调试串口初始化
    My_nsc_api_Init(); //用户自定义api函数初始化
    CANFD0_Init(); // CAN FD0 接口初始化
    CANFD1_Init(); // CAN FD1 接口初始化
    SysTick_Init(100); // 系统时钟初始化，10ms中断一次


    printf("\r\nthis is TrustZone NS World!\r\n");


    // 以太网测试
//    printf("测试以太网功能：\r\n");
//    printf("Hello! FreeRTOS-Plus-TCP\r\n"
//           "这是一个以太网 Ping 示例，请把网线接到开发板和路由器进行联网\r\n");

    /* FreeRTOS IP Initialization: This init initializes the IP stack  */
//    status = FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);
//    if(pdFALSE == status)
//    {
//        printf("FreeRTOS_IPInit Failed\r\n"
//               "Returned Error Code: 0x%lx\r\n", status);
//        __asm("BKPT #0\n"); /* trap upon the error  */
//    }
//
//    printf("网络配置中......\r\n");
//    vTaskDelay(1000); // 等待网络配置完成
//    /* TODO: add your own code here */
////    while(1)
////    {
//        /* Check if Both the Ethernet Link and IP link are UP */
//        if(SUCCESS == isNetworkUp())
//        {
//            /* usr_print_ability is added to avoid multiple UP messages or Down Messages repeating*/
//            if(!(PRINT_UP_MSG_DISABLE & usr_print_ability))
//            {
//                printf("\r\nNetwork is Up\r\n");
//                usr_print_ability |= PRINT_UP_MSG_DISABLE;
//            }
//
//            if(!(PRINT_NWK_USR_MSG_DISABLE & usr_print_ability))
//            {
//#if( ipconfigUSE_DHCP != 0 )
//                /* Display the New IP credentials obtained from the DHCP server */
//                updateDhcpResponseToUsr();
//#endif
//                /* Updated IP credentials on to the RTT console */
////                printf("更新ipconfig:\r\n");
//                print_ipconfig();
//                /* DNS lookup for the Domain name requested. This is Synchronous Activity */
////                dnsQuerryFunc(domain_name);
//            }
//
//            if(!(PRINT_NWK_USR_MSG_DISABLE & usr_print_ability))
//            {
//                printf("\r\nPinging %s:\r\n\r\n",(char *)remote_ip_address);
//            }
//
//            while (usrPingCount < USR_PING_COUNT)
//            {
//                /* Send a ICMP Ping request to the requested IP address
//                 * USR_PING_COUNT (100) is used in this Example Project
//                 * For Continuous testing the count can be increased to bigger number
//                 */
//                status =  vSendPing((char *)remote_ip_address);
//                if(status != pdFALSE)
//                {
//                    ping_data.sent++;
//                    printf("!");    // Ping请求发送成功
//                }
//                else
//                {
//                    ping_data.lost++;
//                    printf(".");
//                }
//                usrPingCount++;
//                /* Add some delay between Pings */
//                vTaskDelay(10);
//            }
//
//            if(!(PRINT_NWK_USR_MSG_DISABLE & usr_print_ability))
//            {
//                print_pingResult();
//                usr_print_ability |= PRINT_NWK_USR_MSG_DISABLE;
//            }
//        }
//        else
//        {
//            if(!(PRINT_DOWN_MSG_DISABLE & usr_print_ability))
//            {
//                 printf("\r\nNetwork is Down");
//                 usr_print_ability |= PRINT_DOWN_MSG_DISABLE;
//            }
//            else
//            {
//                 printf(".");
//            }
//        }
////        vTaskDelay(100);
////    }


    // 测试发送UDP数据包
//    printf("\r\n这是一个以太网 UDP send 示例\r\n");
//    char * BufferToTransmit2 = "UDPtest message";
//    unsigned int TotalLengthToSend2 = strlen(BufferToTransmit2);
//    int sendret2 = vUDPSend(remote_ip_address, BufferToTransmit2, TotalLengthToSend2);
//    if(!sendret2)
//    {
//        printf("UDPtest Success!\r\n");
//    }
//    else
//    {
//        printf("UDPtest Failed!\r\n");
//    }

    /* 测试发送CAN数据 */
    printf("这是一个 CAN FD 通讯例程\r\n");
    printf("打开串口助手发送以下指令，对 CAN-FD 进行相应的操作\r\n");
    printf("\t指令   ------  操作\r\n");
    printf("\t 0   ------  CAN-FD0 发送数据帧\r\n");
    printf("\t 1   ------  CAN-FD1 发送数据帧\r\n");
    printf("\t=======================================\r\n\r\n");

    uint64_t start, end;
    uint64_t cpu_time_used;
    uint64_t frame_num = 2;

    while(1)
    {
        if (true == canfd0_senddata_enable)
        {
            canfd0_senddata_enable = false; //清零标志位

            /* 测试从 CANFD0 发送到 CANFD1 */
            start = system_get_time();

            for(int i = frame_num; i > 0; i--)
            {
                CANFD0_Operation();
            }

            end = system_get_time();
            cpu_time_used = (end - start);
            printf("%u\r\n", (unsigned int)(cpu_time_used & 0xffffffff));

//            printf("等待 CANFD1 接收完成中断\r\n");
            while (false == canfd1_rx_complete_flag);

            canfd1_rx_complete_flag = false;

//            printf("CANFD0 -> CANFD1 开始验证数据\r\n");
            for( uint16_t j = 0; j < canfd0_tx_frame.data_length_code; j++)
            {
                if (canfd0_tx_frame.data[j] != canfd1_rx_frame.data[j])
                {
                    printf("CANFD0发送数据与CANFD1接收数据不一致\r\n");
                }
            }
//            printf("CANFD0 -> CANFD1 测试完成\r\n\r\n");
        }

        if (true == canfd1_senddata_enable)
        {
            canfd1_senddata_enable = false; //清零标志位

            /* 测试从 CANFD1 发送到 CANFD0 */
            start = system_get_time();

            for(int i = frame_num; i > 0; i--)
            {
                CANFD1_Operation();
            }

            end = system_get_time();
            cpu_time_used = (end - start);
            printf("%u\r\n", (unsigned int)(cpu_time_used & 0xffffffff));

//            printf("等待 CANFD0 接收完成中断\r\n");
            while (false == canfd0_rx_complete_flag);

            canfd0_rx_complete_flag = false;

//            printf("CANFD1 -> CANFD0 开始验证数据\r\n");
            for( uint16_t j = 0; j < canfd1_tx_frame.data_length_code; j++)
            {
                if (canfd1_tx_frame.data[j] != canfd0_rx_frame.data[j])
                {
                    printf("CANFD1发送数据与CANFD0接收数据不一致\r\n");
                }
            }
//            printf("CANFD1 -> CANFD0 测试完成\r\n\r\n");
        }
    }

    while(1)
    {
        ;
    }

#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);

#if BSP_CFG_SDRAM_ENABLED

        /* Setup SDRAM and initialize it. Must configure pins first. */
        R_BSP_SdramInit(true);
#endif
    }
}

#if BSP_TZ_SECURE_BUILD

FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
FSP_CPP_FOOTER

#endif

