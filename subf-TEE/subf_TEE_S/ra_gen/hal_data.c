/* generated HAL source file - do not edit */
#include "hal_data.h"
/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd1_bit_timing_cfg =
{
/* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
.baud_rate_prescaler = 1,
  .time_segment_1 = 59, .time_segment_2 = 20, .synchronization_jump_width = 4 };

#if BSP_FEATURE_CANFD_FD_SUPPORT
can_bit_timing_cfg_t g_canfd1_data_timing_cfg =
{
    /* Actual bitrate: 2000000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 14,
    .time_segment_2 = 5,
    .synchronization_jump_width = 1
};
#endif

extern const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM];
#ifndef CANFD_PRV_GLOBAL_CFG
#define CANFD_PRV_GLOBAL_CFG

#ifdef RA_NOT_DEFINED
#undef RA_NOT_DEFINED
#endif
#define RA_NOT_DEFINED (0)

/* Buffer RAM used: RA_NOT_DEFINED bytes */
canfd_global_cfg_t g_canfd_global_cfg =
{ .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES, .global_config = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK
        | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U)
        | CANFD_CFG_FD_OVERFLOW | ((RA_NOT_DEFINED) << R_CANFD_CFDGCFG_ITRCP_Pos)),
  .rx_mb_config = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)), .global_err_ipl =
          CANFD_CFG_GLOBAL_ERR_IPL,
  .rx_fifo_ipl = CANFD_CFG_RX_FIFO_IPL, .rx_fifo_config =
  { ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE)
            | (CANFD_CFG_RXFIFO0_ENABLE)),
    ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE)
            | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
    ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE)
            | (CANFD_CFG_RXFIFO2_ENABLE)),
    ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE)
            | (CANFD_CFG_RXFIFO3_ENABLE)),
    ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE)
            | (CANFD_CFG_RXFIFO4_ENABLE)),
    ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE)
            | (CANFD_CFG_RXFIFO5_ENABLE)),
    ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE)
            | (CANFD_CFG_RXFIFO6_ENABLE)),
    ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE)
            | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
          },
  .common_fifo_config =
  { CANFD_CFG_COMMONFIFO0,
#if !BSP_FEATURE_CANFD_LITE
    CANFD_CFG_COMMONFIFO1,
    CANFD_CFG_COMMONFIFO2, CANFD_CFG_COMMONFIFO3, CANFD_CFG_COMMONFIFO4, CANFD_CFG_COMMONFIFO5,
#endif
          } };
#undef RA_NOT_DEFINED

#endif

canfd_extended_cfg_t g_canfd1_extended_cfg =
{ .p_afl = p_canfd1_afl, .txmb_txi_enable = ((1ULL << 0) | 0ULL), .error_interrupts = (R_CANFD_CFDC_CTR_EWIE_Msk
        | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk
        | R_CANFD_CFDC_CTR_OLIE_Msk | 0U),
#if BSP_FEATURE_CANFD_FD_SUPPORT
    .p_data_timing      = &g_canfd1_data_timing_cfg,
#else
  .p_data_timing = NULL,
#endif
  .delay_compensation = (1),
  .p_global_cfg = &g_canfd_global_cfg, };

canfd_instance_ctrl_t g_canfd1_ctrl;
const can_cfg_t g_canfd1_cfg =
{ .channel = 1, .p_bit_timing = &g_canfd1_bit_timing_cfg, .p_callback = canfd1_callback, .p_extend =
          &g_canfd1_extended_cfg,
  .p_context = NULL, .ipl = (10),
#if defined(VECTOR_NUMBER_CAN1_COMFRX)
    .rx_irq             = VECTOR_NUMBER_CAN1_COMFRX,
#else
  .rx_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_TX)
    .tx_irq             = VECTOR_NUMBER_CAN1_TX,
#else
  .tx_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_CHERR)
    .error_irq             = VECTOR_NUMBER_CAN1_CHERR,
#else
  .error_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const can_instance_t g_canfd1 =
{ .p_ctrl = &g_canfd1_ctrl, .p_cfg = &g_canfd1_cfg, .p_api = &g_canfd_on_canfd };
/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd0_bit_timing_cfg =
{
/* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
.baud_rate_prescaler = 1,
  .time_segment_1 = 59, .time_segment_2 = 20, .synchronization_jump_width = 4 };

#if BSP_FEATURE_CANFD_FD_SUPPORT
can_bit_timing_cfg_t g_canfd0_data_timing_cfg =
{
    /* Actual bitrate: 2000000 Hz. Actual sample point: 75 %. */
    .baud_rate_prescaler = 1,
    .time_segment_1 = 14,
    .time_segment_2 = 5,
    .synchronization_jump_width = 1
};
#endif

extern const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM];
#ifndef CANFD_PRV_GLOBAL_CFG
#define CANFD_PRV_GLOBAL_CFG

#ifdef RA_NOT_DEFINED
#undef RA_NOT_DEFINED
#endif
#define RA_NOT_DEFINED (0)

/* Buffer RAM used: RA_NOT_DEFINED bytes */
canfd_global_cfg_t g_canfd_global_cfg =
{
    .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES,
    .global_config     = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | CANFD_CFG_FD_OVERFLOW |
                          ((RA_NOT_DEFINED) << R_CANFD_CFDGCFG_ITRCP_Pos)),
    .rx_mb_config      = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)),
    .global_err_ipl = CANFD_CFG_GLOBAL_ERR_IPL,
    .rx_fifo_ipl    = CANFD_CFG_RX_FIFO_IPL,
    .rx_fifo_config    =
    {
        ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE) | (CANFD_CFG_RXFIFO0_ENABLE)),
        ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE) | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
        ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE) | (CANFD_CFG_RXFIFO2_ENABLE)),
        ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE) | (CANFD_CFG_RXFIFO3_ENABLE)),
        ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE) | (CANFD_CFG_RXFIFO4_ENABLE)),
        ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE) | (CANFD_CFG_RXFIFO5_ENABLE)),
        ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE) | (CANFD_CFG_RXFIFO6_ENABLE)),
        ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE) | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
    },
    .common_fifo_config =
    {
        CANFD_CFG_COMMONFIFO0,
#if !BSP_FEATURE_CANFD_LITE
        CANFD_CFG_COMMONFIFO1,
        CANFD_CFG_COMMONFIFO2,
        CANFD_CFG_COMMONFIFO3,
        CANFD_CFG_COMMONFIFO4,
        CANFD_CFG_COMMONFIFO5,
#endif
    }
};
#undef RA_NOT_DEFINED

#endif

canfd_extended_cfg_t g_canfd0_extended_cfg =
{ .p_afl = p_canfd0_afl, .txmb_txi_enable = ((1ULL << 0) | 0ULL), .error_interrupts = (R_CANFD_CFDC_CTR_EWIE_Msk
        | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk
        | R_CANFD_CFDC_CTR_OLIE_Msk | 0U),
#if BSP_FEATURE_CANFD_FD_SUPPORT
    .p_data_timing      = &g_canfd0_data_timing_cfg,
#else
  .p_data_timing = NULL,
#endif
  .delay_compensation = (1),
  .p_global_cfg = &g_canfd_global_cfg, };

canfd_instance_ctrl_t g_canfd0_ctrl;
const can_cfg_t g_canfd0_cfg =
{ .channel = 0, .p_bit_timing = &g_canfd0_bit_timing_cfg, .p_callback = canfd0_callback, .p_extend =
          &g_canfd0_extended_cfg,
  .p_context = NULL, .ipl = (10),
#if defined(VECTOR_NUMBER_CAN0_COMFRX)
    .rx_irq             = VECTOR_NUMBER_CAN0_COMFRX,
#else
  .rx_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_TX)
    .tx_irq             = VECTOR_NUMBER_CAN0_TX,
#else
  .tx_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_CHERR)
    .error_irq             = VECTOR_NUMBER_CAN0_CHERR,
#else
  .error_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const can_instance_t g_canfd0 =
{ .p_ctrl = &g_canfd0_ctrl, .p_cfg = &g_canfd0_cfg, .p_api = &g_canfd_on_canfd };
sci_uart_instance_ctrl_t g_uart4_ctrl;

baud_setting_t g_uart4_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart4_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart4_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          },
  .irda_setting =
  { .ircr_bits_b.ire = 0, .ircr_bits_b.irrxinv = 0, .ircr_bits_b.irtxinv = 0, }, };

/** UART interface configuration */
const uart_cfg_t g_uart4_cfg =
{ .channel = 4, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          NULL,
  .p_context = NULL, .p_extend = &g_uart4_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI4_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI4_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI4_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI4_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI4_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart4 =
{ .p_ctrl = &g_uart4_ctrl, .p_cfg = &g_uart4_cfg, .p_api = &g_uart_on_sci };
void g_hal_init(void)
{
    g_common_init ();
}
