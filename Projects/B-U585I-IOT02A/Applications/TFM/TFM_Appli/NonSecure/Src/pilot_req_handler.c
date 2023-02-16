/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "gi_cmd_handler.h"
#include "pilot_req_handler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "psa/error.h"
#include "psa/internal_trusted_storage.h"
#include "utils.h"
extern QueueHandle_t pilot_requests_queue;

#define REQ_HANDLER_DEBUG
/* This function emulates a request event (via mailbox/usb/smb)
 * when a request arrives its content is copied in a buffer whose pointer is put in the request queue
 */
static print_message (host_msg_header *msg) {
#ifdef REQ_HANDLER_DEBUG
  uint32_t *ptr = (uint32_t *)(uintptr_t)msg;
  for(uint32_t i=0; i<msg->size; i++) {
      printf("msg[%d]: 0x%08x\r\n", i, *ptr++);
  }
#endif
}

void task_fake_request (void *pvParameters) {
  uint32_t* fake_request = NULL;
  uint32_t cmd_id = 0;
  const uint8_t master_key[] = {
      0x0, 0x1, 0x2, 0x3,
      0x4, 0x5, 0x6, 0x7,
      0x8, 0x9, 0xA, 0xB,
      0xC, 0xD, 0xE, 0xF,
  };
  /* Test fake message queueuing */
  while (1) {
      host_msg_header header;
      header.tag = SET_API_TAG(HOST_TAG_REQ, HOST_MAIL_VER_1);
      header.req_id = 0xB;
      header.cmd_id = cmd_id;
      switch (cmd_id) {
	case SET_MASTER_KEY_CMD:
	  header.size = sizeof(host_set_master_key_req)/sizeof(uint32_t);
	  fake_request = pvPortMalloc(sizeof(host_set_master_key_req));
	  memcpy(fake_request, &header, sizeof(header));
	  memcpy(((host_set_master_key_req *)fake_request)->key, master_key, sizeof(master_key));
	  break;
	case DPU_LOAD_CMD:
	  header.size = 1;
	  fake_request = pvPortMalloc(sizeof(host_msg_header));
	  memcpy(fake_request, &header, sizeof(header));
	  break;
	default:
	  break;
      }

      if (xQueueSendToBack(pilot_requests_queue, &fake_request, 0) != pdPASS) {
        /* Could not send to the queue */
        Error_Handler();
      }
      cmd_id++;
      if (cmd_id == MAX_CMD_NR) {
	  vTaskSuspend(NULL);
      }
      /* Move the task in the blocked state for 5s */
      vTaskDelay(pdMS_TO_TICKS( 5000 ));
  }
}

static void send_response (host_msg_header *req, host_msg_header *rsp, size_t rsp_size) {
  rsp->tag = SET_API_TAG(HOST_TAG_RSP, HOST_MAIL_VER_1);
  rsp->req_id = req->req_id;
  rsp->cmd_id = req->cmd_id;
  rsp->size = rsp_size / sizeof(uint32_t);

  print_message(rsp);
}
static void set_master_key (host_set_master_key_req *req) {
  const psa_storage_uid_t uid = ITS_MASTER_KEY_UID;
  const psa_storage_create_flags_t flags = PSA_STORAGE_FLAG_WRITE_ONCE;
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  host_set_master_key_rsp rsp;
  memset(&rsp, 0, sizeof(rsp));
  rsp.status = API_FAILURE;

  psa_status = psa_its_set(uid, sizeof(req->key), req->key, flags);
  if (psa_status == PSA_SUCCESS) {
      rsp.status = API_SUCCESS;
  } else if (psa_status == PSA_ERROR_NOT_PERMITTED) {
      rsp.status = SET_API_ERROR(API_STATUS_NOT_PERMITTED);
  }
  send_response (&req->hdr, &rsp.hdr, sizeof(host_set_master_key_rsp));
}

static void dpu_load (host_dpu_load_req *req) {
  host_dpu_load_rsp rsp;
  memset(&rsp, 0, sizeof(rsp));
  rsp.status = API_FAILURE;
  if (gi_dpu_load() == PILOT_SUCCESS) {
      rsp.status = API_SUCCESS;
  }
  send_response (&req->hdr, &rsp.hdr, sizeof(host_dpu_load_rsp));
}

void task_pilot_req_handler (void *pvParameters) {
  host_msg_header *req = 0;
  api_error_t status = API_FAILURE;
  while(1) {
    xQueueReceive( pilot_requests_queue, &req, portMAX_DELAY);
    print_message(req);
    switch (req->cmd_id) {
      case SET_MASTER_KEY_CMD:
	set_master_key((host_set_master_key_req *)req);
	break;
      case DPU_LOAD_CMD:
	dpu_load((host_dpu_load_req *)req);
	break;
      default:
	break;
    }
    vPortFree((void *)req);
  }
}
