/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef NRF_RPC_TR_GENERIC_H_
#define NRF_RPC_TR_GENERIC_H_

#include <stdint.h>
#include "nrf_rpc_os.h"

/**
 * @file
 * @defgroup rp_transport Remote procedures transport
 * @{
 * @brief Remote procedures transport implementation using rpmsg
 *
 * API is compatible with rp_trans API. For API documentation
 * @see nrf_rpc_tr_tmpl.h
 */

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_RPC_TR_HEADER_SIZE 2

#if (CONFIG_NRF_RPC_LOCAL_THREAD_POOL_SIZE <= 8) && (CONFIG_NRF_RPC_REMOTE_THREAD_POOL_SIZE <= 8)
typedef uint8_t nrf_rpc_tr_addr_mask_t;
#elif (CONFIG_NRF_RPC_LOCAL_THREAD_POOL_SIZE <= 16) && (CONFIG_NRF_RPC_REMOTE_THREAD_POOL_SIZE <= 16)
typedef uint16_t nrf_rpc_tr_addr_mask_t;
#else
typedef uint32_t nrf_rpc_tr_addr_mask_t;
RP_STATIC_ASSERT(CONFIG_NRF_RPC_LOCAL_THREAD_POOL_SIZE < 32, "Too many items in local thread pool");
RP_STATIC_ASSERT(CONFIG_NRF_RPC_REMOTE_THREAD_POOL_SIZE < 32, "Too many items in local thread pool");
#endif

struct nrf_rpc_tr_remote_ep {
	struct nrf_rpc_tr_remote_ep *next;
	bool used;
	uint8_t addr;
	nrf_rpc_tr_addr_mask_t addr_mask;
};

struct nrf_rpc_tr_local_ep {
	uint8_t addr;
	nrf_rpc_tr_addr_mask_t addr_mask;
	nrf_rpc_sem_t input_sem;
	nrf_rpc_sem_t done_sem;
	bool wait_for_done;
	bool buffer_owned;
	nrf_rpc_atomic_t input_length;
	const uint8_t* input_buffer;
	void* custom_data;
};

struct nrf_rpc_tr_group {
	uint8_t id;
};


typedef void (*nrf_rpc_tr_receive_handler)(struct nrf_rpc_tr_local_ep *dst_ep,
					   struct nrf_rpc_tr_remote_ep *src_ep,
					   const uint8_t *buf, size_t len);

typedef uint32_t (*nrf_rpc_tr_filter)(struct nrf_rpc_tr_local_ep *dst_ep,
				      struct nrf_rpc_tr_remote_ep *src_ep,
				      const uint8_t *buf, size_t len);


int nrf_rpc_tr_init(nrf_rpc_tr_receive_handler callback,
		    nrf_rpc_tr_filter filter);


#define nrf_rpc_tr_alloc_tx_buf(dst_ep, buf, len)                              \
	(void)(dst_ep);                                                        \
	uint32_t _nrf_rpc_tr_buf_vla[(sizeof(uint32_t) - 1 + (len)) / sizeof(uint32_t)];\
	*(buf) = ((uint8_t *)(&_nrf_rpc_tr_buf_vla)) + NRF_RPC_TR_HEADER_SIZE

#define nrf_rpc_tr_free_tx_buf(dst_ep, buf)

#define nrf_rpc_tr_alloc_failed(buf) 0

int nrf_rpc_tr_send(struct nrf_rpc_tr_local_ep *local_ep, struct nrf_rpc_tr_remote_ep *dst_ep, uint8_t *buf,
		    size_t len);

int nrf_rpc_tr_read(struct nrf_rpc_tr_local_ep *local_ep, struct nrf_rpc_tr_remote_ep **src_ep, const uint8_t **buf);

void nrf_rpc_tr_release_buffer(struct nrf_rpc_tr_local_ep *local_ep);

struct nrf_rpc_tr_remote_ep *nrf_rpc_tr_remote_reserve(void);
void nrf_rpc_tr_remote_release(struct nrf_rpc_tr_remote_ep *ep);

struct nrf_rpc_tr_local_ep *nrf_rpc_tr_current_get();

#ifdef __cplusplus
}
#endif

#endif /* NRF_RPC_TR_RPMSG_H_ */
