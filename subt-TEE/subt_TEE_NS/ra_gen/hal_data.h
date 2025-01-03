/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_canfd.h"
#include "r_can_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
FSP_HEADER
#define FSP_GUARD_g_canfd1_R_CANFD_Open() g_canfd1_open_guard(FSP_SECURE_ARGUMENT, FSP_SECURE_ARGUMENT)

#define FSP_GUARD_g_canfd1_R_CANFD_Close() g_canfd1_close_guard(FSP_SECURE_ARGUMENT)

#define FSP_GUARD_g_canfd1_R_CANFD_Write(buffer, p_frame) g_canfd1_write_guard(FSP_SECURE_ARGUMENT, buffer, p_frame)

#define FSP_GUARD_g_canfd1_R_CANFD_Read(buffer, p_frame) g_canfd1_read_guard(FSP_SECURE_ARGUMENT, buffer, p_frame)

#define FSP_GUARD_g_canfd1_R_CANFD_ModeTransition(operation_mode, test_mode) g_canfd1_mode_transition_guard(FSP_SECURE_ARGUMENT, operation_mode, test_mode)

#define FSP_GUARD_g_canfd1_R_CANFD_InfoGet(p_info) g_canfd1_info_get_guard(FSP_SECURE_ARGUMENT, p_info)

#define FSP_GUARD_g_canfd1_R_CANFD_CallbackSet(p_callback, p_context, p_callback_memory) g_canfd1_callback_set_guard(FSP_SECURE_ARGUMENT, p_callback, p_context, p_callback_memory)

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_open_guard(can_ctrl_t *const p_api_ctrl, can_cfg_t const *const p_cfg);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_close_guard(can_ctrl_t *const p_api_ctrl);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_write_guard(can_ctrl_t *const p_api_ctrl, uint32_t buffer,
        can_frame_t *const p_frame);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_read_guard(can_ctrl_t *const p_api_ctrl, uint32_t buffer,
        can_frame_t *const p_frame);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_mode_transition_guard(can_ctrl_t *const p_api_ctrl,
        can_operation_mode_t operation_mode, can_test_mode_t test_mode);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_info_get_guard(can_ctrl_t *const p_api_ctrl, can_info_t *const p_info);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_callback_set_guard(can_ctrl_t *const p_api_ctrl,
        void (*p_callback)(can_callback_args_t*), void const *const p_context,
        can_callback_args_t *const p_callback_memory);

extern const can_instance_t g_canfd1;
#define FSP_GUARD_g_canfd0_R_CANFD_Open() g_canfd0_open_guard(FSP_SECURE_ARGUMENT, FSP_SECURE_ARGUMENT)

#define FSP_GUARD_g_canfd0_R_CANFD_Close() g_canfd0_close_guard(FSP_SECURE_ARGUMENT)

#define FSP_GUARD_g_canfd0_R_CANFD_Write(buffer, p_frame) g_canfd0_write_guard(FSP_SECURE_ARGUMENT, buffer, p_frame)

#define FSP_GUARD_g_canfd0_R_CANFD_Read(buffer, p_frame) g_canfd0_read_guard(FSP_SECURE_ARGUMENT, buffer, p_frame)

#define FSP_GUARD_g_canfd0_R_CANFD_ModeTransition(operation_mode, test_mode) g_canfd0_mode_transition_guard(FSP_SECURE_ARGUMENT, operation_mode, test_mode)

#define FSP_GUARD_g_canfd0_R_CANFD_InfoGet(p_info) g_canfd0_info_get_guard(FSP_SECURE_ARGUMENT, p_info)

#define FSP_GUARD_g_canfd0_R_CANFD_CallbackSet(p_callback, p_context, p_callback_memory) g_canfd0_callback_set_guard(FSP_SECURE_ARGUMENT, p_callback, p_context, p_callback_memory)

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_open_guard(can_ctrl_t *const p_api_ctrl, can_cfg_t const *const p_cfg);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_close_guard(can_ctrl_t *const p_api_ctrl);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_write_guard(can_ctrl_t *const p_api_ctrl, uint32_t buffer,
        can_frame_t *const p_frame);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_read_guard(can_ctrl_t *const p_api_ctrl, uint32_t buffer,
        can_frame_t *const p_frame);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_mode_transition_guard(can_ctrl_t *const p_api_ctrl,
        can_operation_mode_t operation_mode, can_test_mode_t test_mode);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_info_get_guard(can_ctrl_t *const p_api_ctrl, can_info_t *const p_info);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd0_callback_set_guard(can_ctrl_t *const p_api_ctrl,
        void (*p_callback)(can_callback_args_t*), void const *const p_context,
        can_callback_args_t *const p_callback_memory);

extern const can_instance_t g_canfd0;
#define FSP_GUARD_g_uart4_R_SCI_UART_Open() g_uart4_open_guard(FSP_SECURE_ARGUMENT, FSP_SECURE_ARGUMENT)

#define FSP_GUARD_g_uart4_R_SCI_UART_Read(p_dest, bytes) g_uart4_read_guard(FSP_SECURE_ARGUMENT, p_dest, bytes)

#define FSP_GUARD_g_uart4_R_SCI_UART_Write(p_src, bytes) g_uart4_write_guard(FSP_SECURE_ARGUMENT, p_src, bytes)

#define FSP_GUARD_g_uart4_R_SCI_UART_BaudSet(p_baud_setting) g_uart4_baud_set_guard(FSP_SECURE_ARGUMENT, p_baud_setting)

#define FSP_GUARD_g_uart4_R_SCI_UART_InfoGet(p_info) g_uart4_info_get_guard(FSP_SECURE_ARGUMENT, p_info)

#define FSP_GUARD_g_uart4_R_SCI_UART_Close() g_uart4_close_guard(FSP_SECURE_ARGUMENT)

#define FSP_GUARD_g_uart4_R_SCI_UART_Abort(communication_to_abort) g_uart4_abort_guard(FSP_SECURE_ARGUMENT, communication_to_abort)

#define FSP_GUARD_g_uart4_R_SCI_UART_BaudCalculate(baudrate, bitrate_modulation, baud_rate_error_x_1000, p_baud_setting) g_uart4_baud_calculate_guard(baudrate, bitrate_modulation, baud_rate_error_x_1000, p_baud_setting)

#define FSP_GUARD_g_uart4_R_SCI_UART_CallbackSet(p_callback, p_context, p_callback_memory) g_uart4_callback_set_guard(FSP_SECURE_ARGUMENT, p_callback, p_context, p_callback_memory)

#define FSP_GUARD_g_uart4_R_SCI_UART_ReadStop(remaining_bytes) g_uart4_read_stop_guard(FSP_SECURE_ARGUMENT, remaining_bytes)

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_open_guard(uart_ctrl_t *const p_api_ctrl, uart_cfg_t const *const p_cfg);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_read_guard(uart_ctrl_t *const p_api_ctrl, uint8_t *const p_dest,
        uint32_t const bytes);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_write_guard(uart_ctrl_t *const p_api_ctrl, uint8_t const *const p_src,
        uint32_t const bytes);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_baud_set_guard(uart_ctrl_t *const p_api_ctrl,
        void const *const p_baud_setting);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_info_get_guard(uart_ctrl_t *const p_api_ctrl, uart_info_t *const p_info);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_close_guard(uart_ctrl_t *const p_api_ctrl);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_abort_guard(uart_ctrl_t *const p_api_ctrl, uart_dir_t communication_to_abort);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_baud_calculate_guard(uint32_t baudrate, bool bitrate_modulation,
        uint32_t baud_rate_error_x_1000, baud_setting_t *const p_baud_setting);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_callback_set_guard(uart_ctrl_t *const p_api_ctrl,
        void (*p_callback)(uart_callback_args_t*), void const *const p_context,
        uart_callback_args_t *const p_callback_memory);

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_uart4_read_stop_guard(uart_ctrl_t *const p_api_ctrl, uint32_t *remaining_bytes);

extern const uart_instance_t g_uart4;
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
