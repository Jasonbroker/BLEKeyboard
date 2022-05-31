#include <string.h>

#include "i2c_master.h"
#include "nrfx_twi.h"
#include "print.h"
#include "nrf_pwr_mgmt.h"

//static const nrfx_twi_t m_twi_master = NRFX_TWI_INSTANCE(0);
//
#ifndef CONFIG_PIN_SCL
#define CONFIG_PIN_SCL 3
#endif
#ifndef CONFIG_PIN_SDA
#define CONFIG_PIN_SDA 4
#endif

#ifndef CONFIG_I2C_FREQ
#define CONFIG_I2C_FREQ NRF_TWI_FREQ_400K
#endif

// #include "nrfx_twim.h"

static const nrfx_twi_t m_twi_master = NRFX_TWI_INSTANCE(0);
static int twim_complete = 0;
void twi_evt_handler(nrfx_twi_evt_t const *p_event, void *p_context) {
  NRF_LOG_INFO("TWI Event:%d", p_event->type);
  twim_complete = p_event->type + 1;
}

int i2c_init(void) {
  const nrfx_twi_config_t config =
  {
     .scl                = CONFIG_PIN_SCL,
     .sda                = CONFIG_PIN_SDA,
     .frequency          = CONFIG_I2C_FREQ,
     .interrupt_priority = APP_IRQ_PRIORITY_LOW,
  };
  int res = (int)nrfx_twi_init(&m_twi_master, &config, twi_evt_handler, NULL);
  nrfx_twi_enable(&m_twi_master);
  return res;
}

void i2c_uninit(void) {
  nrfx_twi_uninit(&m_twi_master);
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length, bool no_stop)
{
  nrfx_err_t res;
  twim_complete = 0;
  res = nrfx_twi_tx(&m_twi_master, address, data, length, no_stop);
  int cnt=0;
  if (res == NRFX_SUCCESS) {
    while (twim_complete == 0) {
//      __WFI();
      nrf_pwr_mgmt_run();
      if(cnt++ == 1000) break;
    }
  }
  if(cnt == 1000) NRF_LOG_INFO("TX TIMEOUT");
  return twim_complete == 1 ? 0 : 1;
}