
#include "nrf_log.h"

#define kb_nrf_print(...) NRF_LOG_INFO(__VA_ARGS__)

#define kb_nrf_dprint(...) NRF_LOG_DEBUG(__VA_ARGS__)

void log_init(void);

