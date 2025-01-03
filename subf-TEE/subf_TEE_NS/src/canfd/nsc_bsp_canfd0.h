/*
 * nsc_bsp_canfd0.h
 *
 *  Created on: 2024年10月30日
 *      Author: 27951
 */

#ifndef CANFD_NSC_BSP_CANFD0_H_
#define CANFD_NSC_BSP_CANFD0_H_
#include "hal_data.h"
#include "stdio.h"


// CANFD0模块的调试信息输出控制
#define CANFD0_DEBUG                    1
#if (1 == CANFD0_DEBUG)
  #define CANFD0_MSG_PRINTF(format, ...)  printf("[CANFD0 Operation] "format"\r\n", ##__VA_ARGS__)
#else
  #define CANFD0_MSG_PRINTF(format, ...)
#endif


#define CAN_MAILBOX_NUMBER_0            (CANFD_TX_MB_0)    //邮箱号码
#define CAN_CLASSIC_FRAME_DATA_BYTES    (8U)               //用于经典帧的数据长度代码
#define CAN_FD_DATA_LENGTH_CODE         (64)               //FD帧的数据长度编码
#define CAN_ID                          (0x1100)           //要在传输帧中更新的ID

/* 验收过滤器数组参数 */
#define CANFD_FILTER_ID                 (0x00001000)
#define MASK_ID                         (0x1FFFF000)
#define MASK_ID_MODE                    (1)


#define WAIT_TIME                       (0xFFFF)


void CANFD0_Init(void);
void CAN0_Operation(void);
void CANFD0_Operation(void);
void canfd0_callback(can_callback_args_t * p_args);


#endif /* CANFD_NSC_BSP_CANFD0_H_ */
