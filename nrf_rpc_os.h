#ifndef NRF_RPC_OS_H
#define NRF_RPC_OS_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h> 

typedef sem_t nrf_rpc_sem_t;
typedef uint32_t nrf_rpc_atomic_t;

#define IS_ENABLED(config_macro) _NRF_RPC_IS_ENABLED1(config_macro)
#define _NRF_RPC_IS_ENABLED1(config_macro) _NRF_RPC_IS_ENABLED2(_XXXX##config_macro)
#define _XXXX1 _YYYY,
#define _NRF_RPC_IS_ENABLED2(one_or_two_args) _NRF_RPC_IS_ENABLED3(one_or_two_args true, false)
#define _NRF_RPC_IS_ENABLED3(ignore_this, val, ...) val

#endif /* NRF_RPC_OS_H */
