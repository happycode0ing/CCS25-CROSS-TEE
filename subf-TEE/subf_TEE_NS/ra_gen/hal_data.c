/* generated HAL source file - do not edit */
#include "hal_data.h"
static const can_api_t g_canfd1_nsc_api =
{ .open = g_canfd1_open_guard,
  .close = g_canfd1_close_guard,
  .write = g_canfd1_write_guard,
  .read = g_canfd1_read_guard,
  .modeTransition = g_canfd1_mode_transition_guard,
  .infoGet = g_canfd1_info_get_guard,
  .callbackSet = g_canfd1_callback_set_guard, };
const can_instance_t g_canfd1 =
{ .p_api = &g_canfd1_nsc_api, .p_ctrl = FSP_SECURE_ARGUMENT, .p_cfg = FSP_SECURE_ARGUMENT, };
static const can_api_t g_canfd0_nsc_api =
{ .open = g_canfd0_open_guard,
  .close = g_canfd0_close_guard,
  .write = g_canfd0_write_guard,
  .read = g_canfd0_read_guard,
  .modeTransition = g_canfd0_mode_transition_guard,
  .infoGet = g_canfd0_info_get_guard,
  .callbackSet = g_canfd0_callback_set_guard, };
const can_instance_t g_canfd0 =
{ .p_api = &g_canfd0_nsc_api, .p_ctrl = FSP_SECURE_ARGUMENT, .p_cfg = FSP_SECURE_ARGUMENT, };
static const uart_api_t g_uart4_nsc_api =
{ .open = g_uart4_open_guard,
  .read = g_uart4_read_guard,
  .write = g_uart4_write_guard,
  .baudSet = g_uart4_baud_set_guard,
  .infoGet = g_uart4_info_get_guard,
  .close = g_uart4_close_guard,
  .communicationAbort = g_uart4_abort_guard,
  .callbackSet = g_uart4_callback_set_guard,
  .readStop = g_uart4_read_stop_guard, };
const uart_instance_t g_uart4 =
{ .p_api = &g_uart4_nsc_api, .p_ctrl = FSP_SECURE_ARGUMENT, .p_cfg = FSP_SECURE_ARGUMENT, };
void g_hal_init(void)
{
    g_common_init ();
}
