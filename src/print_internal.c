#include "print.h"
#include "nrf_log_default_backends.h"

/**@brief Function for initializing the nrf log module.
 */
void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}
