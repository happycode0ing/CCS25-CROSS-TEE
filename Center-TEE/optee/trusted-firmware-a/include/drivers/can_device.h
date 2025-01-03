/*
 * Copyright (c) 2021-2024, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include <stdbool.h>
#include <stdint.h>

#include <lib/utils_def.h>

#define CAND_MAX_NUM_INTERFACES			1U
#define CAND_MAX_NUM_CONFIGURATION		1U

#define CAN_LEN_DEV_QUALIFIER_DESC		0x0AU
#define CAN_LEN_DEV_DESC			0x12U
#define CAN_LEN_CFG_DESC			0x09U
#define CAN_LEN_IF_DESC				0x09U
#define CAN_LEN_EP_DESC				0x07U
#define CAN_LEN_OTG_DESC			0x03U
#define CAN_LEN_LANGID_STR_DESC			0x04U
#define CAN_LEN_OTHER_SPEED_DESC_SIZ		0x09U

#define CAND_IDX_LANGID_STR			0x00U
#define CAND_IDX_MFC_STR			0x01U
#define CAND_IDX_PRODUCT_STR			0x02U
#define CAND_IDX_SERIAL_STR			0x03U
#define CAND_IDX_CONFIG_STR			0x04U
#define CAND_IDX_INTERFACE_STR			0x05U
#define CAND_IDX_USER0_STR			0x06U

#define CAN_REQ_TYPE_STANDARD			0x00U
#define CAN_REQ_TYPE_CLASS			0x20U
#define CAN_REQ_TYPE_VENDOR			0x40U
#define CAN_REQ_TYPE_MASK			0x60U

#define CAN_REQ_RECIPIENT_DEVICE		0x00U
#define CAN_REQ_RECIPIENT_INTERFACE		0x01U
#define CAN_REQ_RECIPIENT_ENDPOINT		0x02U
#define CAN_REQ_RECIPIENT_MASK			0x1FU

#define CAN_REQ_DIRECTION			0x80U

#define CAN_REQ_GET_STATUS			0x00U
#define CAN_REQ_CLEAR_FEATURE			0x01U
#define CAN_REQ_SET_FEATURE			0x03U
#define CAN_REQ_SET_ADDRESS			0x05U
#define CAN_REQ_GET_DESCRIPTOR			0x06U
#define CAN_REQ_SET_DESCRIPTOR			0x07U
#define CAN_REQ_GET_CONFIGURATION		0x08U
#define CAN_REQ_SET_CONFIGURATION		0x09U
#define CAN_REQ_GET_INTERFACE			0x0AU
#define CAN_REQ_SET_INTERFACE			0x0BU
#define CAN_REQ_SYNCH_FRAME			0x0CU

#define CAN_DESC_TYPE_DEVICE			0x01U
#define CAN_DESC_TYPE_CONFIGURATION		0x02U
#define CAN_DESC_TYPE_STRING			0x03U
#define CAN_DESC_TYPE_INTERFACE			0x04U
#define CAN_DESC_TYPE_ENDPOINT			0x05U
#define CAN_DESC_TYPE_DEVICE_QUALIFIER		0x06U
#define CAN_DESC_TYPE_OTHER_SPEED_CONFIGURATION	0x07U
#define CAN_DESC_TYPE_BOS			0x0FU

#define CAN_CONFIG_REMOTE_WAKEUP		2U
#define CAN_CONFIG_SELF_POWERED			1U

#define CAN_MAX_EP0_SIZE			64U

/* Device Status */
#define CAND_STATE_DEFAULT			1U
#define CAND_STATE_ADDRESSED			2U
#define CAND_STATE_CONFIGURED			3U
#define CAND_STATE_SUSPENDED			4U

/* EP0 State */
#define CAND_EP0_IDLE				0U
#define CAND_EP0_SETUP				1U
#define CAND_EP0_DATA_IN			2U
#define CAND_EP0_DATA_OUT			3U
#define CAND_EP0_STATUS_IN			4U
#define CAND_EP0_STATUS_OUT			5U
#define CAND_EP0_STALL				6U

#define CAND_EP_TYPE_CTRL			0U
#define CAND_EP_TYPE_ISOC			1U
#define CAND_EP_TYPE_BULK			2U
#define CAND_EP_TYPE_INTR			3U

#define  CAND_OUT_EPNUM_MASK			GENMASK(15, 0)
#define  CAND_OUT_COUNT_MASK			GENMASK(31, 16)
#define  CAND_OUT_COUNT_SHIFT			16U

/* Number of EP supported, allow to reduce footprint: default max = 15 */
#ifndef CONFIG_CAND_EP_NB
#define  CAND_EP_NB				15U
#else
#define  CAND_EP_NB				CONFIG_CAND_EP_NB
#endif

#define LOBYTE(x)	((uint8_t)((x) & 0x00FF))
#define HIBYTE(x)	((uint8_t)(((x) & 0xFF00) >> 8))

struct can_setup_req {
	uint8_t bm_request;
	uint8_t b_request;
	uint16_t value;
	uint16_t index;
	uint16_t length;
};

struct can_handle;

struct can_class {
	uint8_t (*init)(struct can_handle *pdev, uint8_t cfgidx);
	uint8_t (*de_init)(struct can_handle *pdev, uint8_t cfgidx);
	/* Control Endpoints */
	uint8_t (*setup)(struct can_handle *pdev, struct can_setup_req *req);
	uint8_t (*ep0_tx_sent)(struct can_handle *pdev);
	uint8_t (*ep0_rx_ready)(struct can_handle *pdev);
	/* Class Specific Endpoints */
	uint8_t (*data_in)(struct can_handle *pdev, uint8_t epnum);
	uint8_t (*data_out)(struct can_handle *pdev, uint8_t epnum);
	uint8_t (*sof)(struct can_handle *pdev);
	uint8_t (*iso_in_incomplete)(struct can_handle *pdev, uint8_t epnum);
	uint8_t (*iso_out_incomplete)(struct can_handle *pdev, uint8_t epnum);
};

/* Following CAN Device status */
enum can_status {
	CAND_OK = 0U,
	CAND_BUSY,
	CAND_FAIL,
	CAND_TIMEOUT
};

/* Action to do after IT handling */
enum can_action {
	CAN_NOTHING = 0U,
	CAN_DATA_OUT,
	CAN_DATA_IN,
	CAN_SETUP,
	CAN_ENUM_DONE,
	CAN_READ_DATA_PACKET,
	CAN_READ_SETUP_PACKET,
	CAN_RESET,
	CAN_RESUME,
	CAN_SUSPEND,
	CAN_LPM,
	CAN_SOF,
	CAN_DISCONNECT,
	CAN_WRITE_EMPTY
};

/* CAN Device descriptors structure */
struct can_desc {
	uint8_t *(*get_device_desc)(uint16_t *length);
	uint8_t *(*get_lang_id_desc)(uint16_t *length);
	uint8_t *(*get_manufacturer_desc)(uint16_t *length);
	uint8_t *(*get_product_desc)(uint16_t *length);
	uint8_t *(*get_serial_desc)(uint16_t *length);
	uint8_t *(*get_configuration_desc)(uint16_t *length);
	uint8_t *(*get_interface_desc)(uint16_t *length);
	uint8_t *(*get_usr_desc)(uint8_t index, uint16_t *length);
	uint8_t *(*get_config_desc)(uint16_t *length);
	uint8_t *(*get_device_qualifier_desc)(uint16_t *length);
	/* optional: high speed capable device operating at its other speed */
	uint8_t *(*get_other_speed_config_desc)(uint16_t *length);
};

/* CAN Device handle structure */
struct can_endpoint {
	uint32_t status;
	uint32_t total_length;
	uint32_t rem_length;
	uint32_t maxpacket;
};

/*
 * EndPoint descriptor
 * num : Endpoint number, between 0 and 15 (limited by CAND_EP_NB)
 * is_in: Endpoint direction
 * type : Endpoint type
 * maxpacket:  Endpoint Max packet size: between 0 and 64KB
 * xfer_buff: Pointer to transfer buffer
 * xfer_len: Current transfer lengt
 * hxfer_count: Partial transfer length in case of multi packet transfer
 */
struct cand_ep {
	uint8_t num;
	bool is_in;
	uint8_t type;
	uint32_t maxpacket;
	uint8_t *xfer_buff;
	uint32_t xfer_len;
	uint32_t xfer_count;
};

enum pcd_lpm_state {
	LPM_L0 = 0x00U, /* on */
	LPM_L1 = 0x01U, /* LPM L1 sleep */
	LPM_L2 = 0x02U, /* suspend */
	LPM_L3 = 0x03U, /* off */
};

/* CAN Device descriptors structure */
struct can_driver {
	enum can_status (*ep0_out_start)(void *handle);
	enum can_status (*ep_start_xfer)(void *handle, struct cand_ep *ep);
	enum can_status (*ep0_start_xfer)(void *handle, struct cand_ep *ep);
	enum can_status (*write_packet)(void *handle, uint8_t *src,
				     uint8_t ch_ep_num, uint16_t len);
	void *(*read_packet)(void *handle, uint8_t *dest, uint16_t len);
	enum can_status (*ep_set_stall)(void *handle, struct cand_ep *ep);
	enum can_status (*start_device)(void *handle);
	enum can_status (*stop_device)(void *handle);
	enum can_status (*set_address)(void *handle, uint8_t address);
	enum can_status (*write_empty_tx_fifo)(void *handle,
					    uint32_t epnum, uint32_t xfer_len,
					    uint32_t *xfer_count,
					    uint32_t maxpacket,
					    uint8_t **xfer_buff);
	enum can_action (*it_handler)(void *handle, uint32_t *param);
};

/* CAN Peripheral Controller Drivers */
struct pcd_handle {
	void *instance; /* Register base address */
	struct cand_ep in_ep[CAND_EP_NB]; /* IN endpoint parameters */
	struct cand_ep out_ep[CAND_EP_NB]; /* OUT endpoint parameters */
	uint32_t setup[12]; /* Setup packet buffer */
	enum pcd_lpm_state lpm_state; /* LPM State */
};

/* CAN Device handle structure */
struct can_handle {
	uint8_t id;
	uint32_t dev_config;
	uint32_t dev_config_status;
	struct can_endpoint ep_in[CAND_EP_NB];
	struct can_endpoint ep_out[CAND_EP_NB];
	uint32_t ep0_state;
	uint32_t ep0_data_len;
	uint8_t dev_state;
	uint8_t dev_old_state;
	uint8_t dev_address;
	uint32_t dev_remote_wakeup;
	struct can_setup_req request;
	const struct can_desc *desc;
	struct can_class *class;
	void *class_data;
	void *user_data;
	struct pcd_handle *data;
	const struct can_driver *driver;
};

enum can_status can_core_handle_it(struct can_handle *pdev);
enum can_status can_core_receive(struct can_handle *pdev, uint8_t ep_addr,
				 uint8_t *p_buf, uint32_t len);
enum can_status can_core_transmit(struct can_handle *pdev, uint8_t ep_addr,
				  uint8_t *p_buf, uint32_t len);
enum can_status can_core_receive_ep0(struct can_handle *pdev, uint8_t *p_buf,
				     uint32_t len);
enum can_status can_core_transmit_ep0(struct can_handle *pdev, uint8_t *p_buf,
				      uint32_t len);
void can_core_ctl_error(struct can_handle *pdev);
enum can_status can_core_start(struct can_handle *pdev);
enum can_status can_core_stop(struct can_handle *pdev);
enum can_status register_can_driver(struct can_handle *pdev,
				    struct pcd_handle *pcd_handle,
				    const struct can_driver *driver,
				    void *driver_handle);
enum can_status register_platform(struct can_handle *pdev,
				  const struct can_desc *plat_call_back);
// added by lss
// void can_driver_init(void);

#endif /* CAN_DEVICE_H */
