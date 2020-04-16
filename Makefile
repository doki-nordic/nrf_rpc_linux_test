#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#


ifeq (1,$(DEBUG))
    CFLAGS=-g -O0
    STRIP=echo Skipping strip
else
    CFLAGS=-O3
    STRIP=strip
endif

CFLAGS+= \
	-I/dk/ncs/nrfxlib/nrf_rpc/include \
\
	-DCONFIG_NRF_RPC_TR_CUSTOM=\"nrf_rpc_tr_generic.h\" \
	-DCONFIG_NRF_RPC_LOCAL_THREAD_POOL_SIZE=4 \
	-DCONFIG_NRF_RPC_REMOTE_THREAD_POOL_SIZE=4 \
	-DCONFIG_NRF_RPC_EXTRA_EP_COUNT=8 \
	-DCONFIG_NRF_RPC_REMOTE_EXTRA_EP_COUNT=8 \

CSRC= \
	/dk/ncs/nrfxlib/nrf_rpc/nrf_rpc.c

all: nrf_rpc_linux_test

clean:
	rm -f nrf_rpc_linux_test

nrf_rpc_linux_test: Makefile ../nrf_rpc_linux_test/*.c *.h $(CSRC)
	gcc $(CFLAGS) -o $@ -I. $(filter %.c,$^) -lpthread -lrt
	$(STRIP) $@
