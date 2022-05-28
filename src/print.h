
#pragma once

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define printf(...) NRF_LOG_INFO(__VA_ARGS__)

#define dprintf(...) NRF_LOG_DEBUG(__VA_ARGS__)

void log_init(void);
