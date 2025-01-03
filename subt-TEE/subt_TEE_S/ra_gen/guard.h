/* generated guard header file - do not edit */
#ifndef GUARD_H_
#define GUARD_H_
#include "hal_data.h"
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
#endif /* GUARD_H_ */
