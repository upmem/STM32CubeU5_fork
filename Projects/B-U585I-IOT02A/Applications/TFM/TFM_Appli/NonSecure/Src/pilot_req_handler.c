/**
 *
 * Copyright (c) 2022-2023 UPMEM.
 *
 */
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "gi.h"
#include "pilot_req_handler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "psa/error.h"
#include "psa/internal_trusted_storage.h"
#include "pilot_keys.h"
#include "utils.h"

extern QueueHandle_t pilot_requests_queue;

#ifdef DEBUG
#define REQ_HANDLER_DEBUG
#endif

static void print_message (host_msg_header *msg) {
#ifdef REQ_HANDLER_DEBUG
  uint32_t *ptr = (uint32_t *)(uintptr_t)msg;
  printf("\r\n");
  for(uint32_t i=0; i<msg->size; i++) {
      printf("msg[%d]: 0x%08x\r\n", i, *ptr++);
  }
#endif
}

/* This function emulates an Host request received via mailbox/usb/smb
 * when the message arrives its content is copied in a buffer whose pointer is put in the request queue
 * TODO remove this function
 */
void task_fake_request (void *pvParameters) {
  uint32_t* fake_request = NULL;
  uint32_t cmd_id = SET_MASTER_KEY_CMD;
  const uint8_t master_key[AES_128_KEY_SIZE] = {
      0x0, 0x1, 0x2, 0x3,
      0x4, 0x5, 0x6, 0x7,
      0x8, 0x9, 0xA, 0xB,
      0xC, 0xD, 0xE, 0xF,
  };
  const uint8_t server_pub_key[ECDSA_P256_PUB_KEY_SIZE] = {
      0x0, 0x1, 0x2, 0x3,
      0x4, 0x5, 0x6, 0x7,
      0x8, 0x9, 0xA, 0xB,
      0xC, 0xD, 0xE, 0xF,
      0xF, 0xE, 0xD, 0xC,
      0xB, 0xA, 0x9, 0x8,
      0x7, 0x6, 0x5, 0x4,
      0x3, 0x2, 0x1, 0x0,
      0x0, 0x1, 0x2, 0x3,
      0x4, 0x5, 0x6, 0x7,
      0x8, 0x9, 0xA, 0xB,
      0xC, 0xD, 0xE, 0xF,
      0xF, 0xE, 0xD, 0xC,
      0xB, 0xA, 0x9, 0x8,
      0x7, 0x6, 0x5, 0x4,
      0x3, 0x2, 0x1, 0x0,
  };
  /* Test fake message queueuing */
  while (1) {
      host_msg_header header;
      header.tag_ver = SET_API_TAG_VER(CMD_TAG_REQ, CMD_VERSION_1);
      header.req_id = 0xB;
      header.cmd_id = cmd_id;
      switch (cmd_id) {
	case SET_MASTER_KEY_CMD:
	  header.size = sizeof(host_set_master_key_req)/sizeof(uint32_t);
	  fake_request = pvPortMalloc(sizeof(host_set_master_key_req));
	  memcpy(fake_request, &header, sizeof(header));
	  memcpy(((host_set_master_key_req *)fake_request)->key, master_key, sizeof(master_key));
	  break;
	case SET_SERVER_PUB_KEY_CMD:
	  header.size = sizeof(host_set_server_pub_key_req)/sizeof(uint32_t);
	  fake_request = pvPortMalloc(sizeof(host_set_server_pub_key_req));
	  memcpy(fake_request, &header, sizeof(header));
	  memcpy(((host_set_server_pub_key_req *)fake_request)->key, server_pub_key, sizeof(server_pub_key));
	  break;
	case DPU_LOAD_CMD:
	  header.size = sizeof(host_dpu_load_req)/sizeof(uint32_t) ;
	  fake_request = pvPortMalloc(sizeof(host_dpu_load_req));
	  memcpy(fake_request, &header, sizeof(header));
	  break;
	default:
	  break;
      }

      if (xQueueSendToBack(pilot_requests_queue, &fake_request, 0) != pdPASS) {
        /* Could not send to the queue */
        Error_Handler();
      }
      if (cmd_id == DPU_LOAD_CMD) {
	  vTaskSuspend(NULL);
      }
      cmd_id++;

      /* Move the task to blocked state for 5s */
      vTaskDelay(pdMS_TO_TICKS( 5000 ));
  }
}

/*
 * This is a draft implementation
 * this function is suppose to reply to the Host over the interface used to send the request
 */
static void send_response (host_msg_header *req, host_msg_header *rsp, size_t rsp_size) {
  /* TODO to be finalized*/
  rsp->tag_ver = SET_API_TAG_VER(CMD_TAG_RSP, CMD_VERSION_1);
  rsp->req_id = req->req_id;
  rsp->cmd_id = req->cmd_id;
  rsp->size = rsp_size / sizeof(uint32_t);

  print_message(rsp);
}

static void set_key (psa_storage_uid_t uid, uint8_t *key, size_t key_size, uint16_t *status) {
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  *status = API_FAILURE;
  do {
    if (
	(key[0] == 0) &&
	(memcmp (key, &key[1], key_size -1) == 0)
    ) {
	/* do not accept 0s value keys */
	break;
    }
    psa_status = psa_its_set(uid, key_size, key, PSA_STORAGE_FLAG_WRITE_ONCE);

    if (psa_status != PSA_SUCCESS) {
	if (psa_status == PSA_ERROR_NOT_PERMITTED) {
	    *status = SET_API_ERROR(API_STATUS_NOT_PERMITTED);
	}
	break;
    }
    *status = API_SUCCESS;

  } while(0);
}

static void set_master_key (host_set_master_key_req *req) {
  host_set_master_key_rsp rsp;
  memset(&rsp, 0, sizeof(rsp));
  set_key(ITS_MASTER_KEY_UID, req->key, sizeof(req->key), &rsp.status);
  send_response (&req->hdr, &rsp.hdr, sizeof(host_set_master_key_rsp));
}

static void set_server_pub_key (host_set_server_pub_key_req *req) {
  host_set_server_pub_key_rsp rsp;
  memset(&rsp, 0, sizeof(rsp));
  set_key(ITS_SERVER_PUB_KEY_UID, req->key, sizeof(req->key), &rsp.status);
  send_response (&req->hdr, &rsp.hdr, sizeof(host_set_server_pub_key_rsp));
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

static pilot_error_t check_message_header (host_msg_header *req) {
  pilot_error_t ret = PILOT_FAILURE;
  print_message(req);
  if (
      (req->tag_ver == SET_API_TAG_VER(CMD_TAG_REQ, CMD_VERSION_1)) &&
      (req->size <= MSG_MAX_WORDS_SIZE)
     ) {
      ret = PILOT_SUCCESS;
  }
  return ret;
}

void task_pilot_req_handler (void *pvParameters) {
  host_msg_header *req = 0;
  while(1) {
    xQueueReceive( pilot_requests_queue, &req, portMAX_DELAY);
    if (check_message_header(req) == PILOT_SUCCESS) {
      switch (req->cmd_id) {
	case SET_MASTER_KEY_CMD:
	  set_master_key((host_set_master_key_req *)req);
	  break;
	case SET_SERVER_PUB_KEY_CMD:
	  set_server_pub_key((host_set_server_pub_key_req *)req);
	  break;
	case DPU_LOAD_CMD:
	  dpu_load((host_dpu_load_req *)req);
	  break;
	default:
	  break;
      }
    }
    vPortFree((void *)req);
  }
}
