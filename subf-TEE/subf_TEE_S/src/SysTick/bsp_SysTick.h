/*
 * bsp_SysTick.h
 *
 *  Created on: 2024年12月10日
 *      Author: 27951
 */

#ifndef SYSTICK_BSP_SYSTICK_H_
#define SYSTICK_BSP_SYSTICK_H_

#include "hal_data.h"


typedef enum
{
    SYS_DELAY_UNITS_SECONDS      = 200000000, ///< Requested delay amount is in seconds
    SYS_DELAY_UNITS_MILLISECONDS = 200000,    ///< Requested delay amount is in milliseconds
    SYS_DELAY_UNITS_MICROSECONDS = 200        ///< Requested delay amount is in microseconds
} sys_delay_units_t;


void SysTick_Init(uint32_t IT_frequency);
//void SysTick_Delay(uint32_t delay, sys_delay_units_t unit);
uint64_t system_get_time(void);


#endif /* SYSTICK_BSP_SYSTICK_H_ */
