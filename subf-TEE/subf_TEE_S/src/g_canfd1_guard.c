#include "guard.h"
#include "r_canfd.h"

#define CANFD_TRANSMIT_MAX 20

extern volatile uint64_t canfd_transmission_num;

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_open_guard(can_ctrl_t *const p_api_ctrl, can_cfg_t const *const p_cfg)
{
    /* TODO: add your own security checks here */

    FSP_PARAMETER_NOT_USED (p_api_ctrl);
    FSP_PARAMETER_NOT_USED (p_cfg);

    return R_CANFD_Open (&g_canfd1_ctrl, &g_canfd1_cfg);
}

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_close_guard(can_ctrl_t *const p_api_ctrl)
{
    /* TODO: add your own security checks here */

    FSP_PARAMETER_NOT_USED (p_api_ctrl);

    return R_CANFD_Close (&g_canfd1_ctrl);
}

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_write_guard(can_ctrl_t *const p_api_ctrl, uint32_t buffer,
        can_frame_t *const p_frame)
{
    /* Verify all pointers are in non-secure memory. */
    can_frame_t *const p_frame_checked = cmse_check_pointed_object (p_frame, CMSE_AU_NONSECURE);
    FSP_ASSERT (p_frame_checked != NULL);

    /* TODO: add your own security checks here */
    if(canfd_transmission_num > CANFD_TRANSMIT_MAX)
    {
        return FSP_ERR_CAN_FREQUENCY_HIGN;
    }

    FSP_PARAMETER_NOT_USED (p_api_ctrl);

    return R_CANFD_Write (&g_canfd1_ctrl, buffer, p_frame_checked);
}

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_read_guard(can_ctrl_t *const p_api_ctrl, uint32_t buffer,
        can_frame_t *const p_frame)
{
    /* Verify all pointers are in non-secure memory. */
    can_frame_t *const p_frame_checked = cmse_check_pointed_object (p_frame, CMSE_AU_NONSECURE);
    FSP_ASSERT (p_frame_checked != NULL);

    /* TODO: add your own security checks here */

    FSP_PARAMETER_NOT_USED (p_api_ctrl);

    return R_CANFD_Read (&g_canfd1_ctrl, buffer, p_frame_checked);
}

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_mode_transition_guard(can_ctrl_t *const p_api_ctrl,
        can_operation_mode_t operation_mode, can_test_mode_t test_mode)
{
    /* TODO: add your own security checks here */

    FSP_PARAMETER_NOT_USED (p_api_ctrl);

    return R_CANFD_ModeTransition (&g_canfd1_ctrl, operation_mode, test_mode);
}

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_info_get_guard(can_ctrl_t *const p_api_ctrl, can_info_t *const p_info)
{
    /* Verify all pointers are in non-secure memory. */
    can_info_t *const p_info_checked = cmse_check_address_range ((void*) p_info, sizeof(can_info_t), CMSE_AU_NONSECURE);
    FSP_ASSERT (p_info_checked != NULL);

    /* TODO: add your own security checks here */

    FSP_PARAMETER_NOT_USED (p_api_ctrl);

    return R_CANFD_InfoGet (&g_canfd1_ctrl, p_info_checked);
}

BSP_CMSE_NONSECURE_ENTRY fsp_err_t g_canfd1_callback_set_guard(can_ctrl_t *const p_api_ctrl,
        void (*p_callback)(can_callback_args_t*), void const *const p_context,
        can_callback_args_t *const p_callback_memory)
{
    /* Verify all pointers are in non-secure memory. */
    void (*p_callback_checked)(
            can_callback_args_t*) = (void(*)(can_callback_args_t *)) cmse_check_address_range((void *) p_callback, sizeof(void *), CMSE_AU_NONSECURE);
    FSP_ASSERT(p_callback_checked != NULL);
    can_callback_args_t *const p_callback_memory_checked = cmse_check_pointed_object(p_callback_memory, CMSE_AU_NONSECURE);
    FSP_ASSERT(p_callback_memory_checked != NULL);

    /* TODO: add your own security checks here */

    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_context);

    return R_CANFD_CallbackSet(&g_canfd1_ctrl, p_callback_checked, p_context, p_callback_memory_checked);
}
