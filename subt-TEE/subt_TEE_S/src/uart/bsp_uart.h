/*
 * bsp_uart.h
 *
 *  Created on: 2024年9月24日
 *      Author: 27951
 */

#ifndef UART_BSP_UART_H_
#define UART_BSP_UART_H_
#include "hal_data.h"
#include "stdio.h"

#include <sys/stat.h>
 #include <unistd.h>
 #include <errno.h>

void Debug_UART4_Init(void);
void debug_uart4_callback(uart_callback_args_t * p_args);


#endif /* UART_BSP_UART_H_ */
