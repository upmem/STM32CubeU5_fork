/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __PILOT_REQ_HANDLER_H__
#define __PILOT_REQ_HANDLER_H__

#define HOST_TAG_REQ		(0xA)
#define HOST_TAG_RSP		(0x9)

#define HOST_MAIL_VER_1		(0x1)
#define SET_API_TAG(tag, version) (((tag & 0xf) << 4) | (version & 0xf))

#define SET_MASTER_KEY_CMD	(0x0)
#define DPU_LOAD_CMD		(0x1)
#define MAX_CMD_NR		(0x2)

typedef uint16_t api_error_t;

#define API_SUCCESS			(0x0)
#define API_FAILURE			(0xff)

#define API_STATUS_NOT_PERMITTED	(0x1)

#define SET_API_ERROR(status)		(((status & 0xFF) << 8) | (API_FAILURE))
#define SET_API_WARNING(status) 	(((status & 0xFF) << 8) | (API_SUCCESS))

// TODO check the endianess and adapt the doc
typedef struct {
  uint8_t tag;
  uint8_t req_id;
  uint8_t size;
  uint8_t cmd_id;
} host_msg_header;

typedef struct {
  host_msg_header hdr;
  uint8_t key[16];
} host_set_master_key_req;

typedef struct {
  host_msg_header hdr;
  uint16_t status;
  uint16_t pad;
} host_set_master_key_rsp;

typedef struct {
  host_msg_header hdr;
} host_dpu_load_req;

typedef struct {
  host_msg_header hdr;
  uint16_t status;
  uint16_t pad;
} host_dpu_load_rsp;

void task_fake_request (void *pvParameters);
void task_pilot_req_handler (void *pvParameters);

#endif
