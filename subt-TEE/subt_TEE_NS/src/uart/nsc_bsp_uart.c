/*
 * nsc_bsp_uart.c
 *
 *  Created on: 2024年9月2日
 *      Author: 27951
 */
#include "nsc_bsp_uart.h"

uart_callback_args_t g_uart_callback_args;

// 调试串口 UART4 初始化
void Debug_UART4_Init(void)
{
    fsp_err_t err;
    err = FSP_GUARD_g_uart4_R_SCI_UART_Open();

    err = FSP_GUARD_g_uart4_R_SCI_UART_CallbackSet(debug_uart4_callback, NULL, &g_uart_callback_args);

    assert(FSP_SUCCESS == err);
}

// 发送完成标志
volatile bool uart_send_complete_flag = false;
volatile bool uart_receive_complete_flag = false;

// CANFD 功能操作允许标志位
volatile bool canfd0_senddata_enable = false;  // 允许 CANFD0 发送数据
volatile bool canfd1_senddata_enable = false;  // 允许 CANFD1 发送数据

// 串口中断回调函数
void debug_uart4_callback(uart_callback_args_t * p_args)
{
   switch (p_args->event)
   {
      case UART_EVENT_RX_CHAR:
      {
          /* 根据字符指令进行操作 */
          switch (p_args->data)
          {
              case '0':
                  canfd0_senddata_enable = true;
                  break;
              case '1':
                  canfd1_senddata_enable = true;
                  break;
              default:
                  break;
          }
          // R_SCI_UART_Write(&g_uart4_ctrl, (uint8_t *)&(p_args->data), 1);//回显功能
          break;
      }
      case UART_EVENT_TX_COMPLETE:
      {
          uart_send_complete_flag = true;
          break;
      }
      default:
          break;
   }
}

// 重定向printf输出到串口
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //防止编译警告
int _read(int fd, char *pBuffer, int size);

int _write(int fd, char *pBuffer, int size)
{
   (void)fd;
   fsp_err_t err;
   err = FSP_GUARD_g_uart4_R_SCI_UART_Write((uint8_t *)pBuffer, (uint32_t)size);

   assert(FSP_SUCCESS == err);

   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return size;
}

// 重定向scanf函数
int _read(int fd, char *pBuffer, int size)
{
    (void)fd;
    FSP_GUARD_g_uart4_R_SCI_UART_Read((uint8_t *)pBuffer, (uint32_t)size);
    while(uart_receive_complete_flag == false);
    uart_receive_complete_flag = false;

    // 回显
    FSP_GUARD_g_uart4_R_SCI_UART_Write((uint8_t *)pBuffer, (uint32_t)size);

    return size;
}

#else
int fputc(int ch, FILE *f)
{
   (void)f;
   FSP_GUARD_g_uart4_R_SCI_UART_Write((uint8_t *)&ch, 1);
   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return ch;
}
#endif

/* 函数声明 防止编译器警告 */
int _isatty(int fd);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _fstat(int fd, struct stat *st);

__attribute__((weak)) int _isatty(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

__attribute__((weak)) int _close(int fd)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _lseek(int fd, int ptr, int dir)
{
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}

__attribute__((weak)) int _fstat(int fd, struct stat *st)
{
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}


