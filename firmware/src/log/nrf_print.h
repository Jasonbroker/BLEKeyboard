
#include "nrf_log.h"

#define nrf_print(...) NRF_LOG_INFO(__VA_ARGS__)

#define nrf_dprint(...) NRF_LOG_DEBUG(__VA_ARGS__)

void log_init(void);

