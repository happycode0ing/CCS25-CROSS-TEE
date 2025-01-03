/*
 * bsp_SysTick.c
 *
 *  Created on: 2024年12月10日
 *      Author: 27951
 */
#include "bsp_SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_api.h"

extern volatile uint64_t IT_nums;

//static __IO uint32_t IT_Nums = 0;  //触发中断的次数
static uint32_t IT_Period;  //中断周期(单位为时钟节拍数)
/**
 * @brief  启动系统滴答计时器 SysTick
 * @param  IT_frequency: 滴答计时器每秒的中断次数
 * @retval 无
 */
void SysTick_Init(uint32_t IT_frequency)
{
    /* SystemFrequency在这里默认为200M
     * SystemFrequency / 100    10ms中断一次
     * SystemFrequency / 1000    1ms中断一次
     * SystemFrequency / 100000  10us中断一次
     * SystemFrequency / 1000000 1us中断一次
     */
    IT_Period = SystemCoreClock / IT_frequency;
    uint32_t err = SysTick_Config (IT_Period);
    assert(err == 0); //capture error
}

/**
 * @brief  延时程序
 * @param  delay: 延时的单位时间
 * @param  unit: 延时的单位
 * @retval 无
 */

//void SysTick_Delay(uint32_t delay, sys_delay_units_t unit)
//{
//    uint32_t Sum_Time = delay * unit; //计算总延时时间(单位为时钟节拍数)
//    IT_Nums = Sum_Time / IT_Period;     //计算进入中断的次数
//    SysTick->VAL = 0UL;   //计数清零
//    while (IT_Nums != 0)
//        ;
//}

/**
 * @brief  计算系统从启动后到现在过了多少ms
 * @param  无
 * @retval t_ms: 毫秒数
 */

uint64_t system_get_time(void)
{
    uint64_t t_ns = IT_nums * 10 * 1000000;
    uint64_t t_now = SysTick->VAL;
    uint64_t t_load = SysTick->LOAD;
    uint64_t t_cnt = t_load + 1 - t_now;
    t_ns += t_cnt * 10 * 1000000 / (t_load + 1);

    return t_ns;
}
