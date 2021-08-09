
#include "powermgr.h"
#include "app_timer.h"
#include "nrf_saadc.h"
#include "config.h"
#include "kb_nrf_print.h"
#include "nrfx_saadc.h"

#define MEASURE_INTERVAL APP_TIMER_TICKS(1000)

static bool saadc_initialized = false;

static nrf_saadc_channel_config_t kb_battery_channel_config = {
    .resistor_p = NRF_SAADC_RESISTOR_DISABLED,
    .resistor_n = NRF_SAADC_RESISTOR_DISABLED,
    .gain = NRF_SAADC_GAIN1_2,
    .reference = NRF_SAADC_REFERENCE_INTERNAL,
    .acq_time = NRF_SAADC_ACQTIME_10US,
    .mode = NRF_SAADC_MODE_SINGLE_ENDED,
    .burst = NRF_SAADC_BURST_DISABLED,
    .pin_p = (nrf_saadc_input_t)(BATTERY_PIN),
    .pin_n = NRF_SAADC_INPUT_DISABLED
};

static const nrfx_saadc_config_t config = NRFX_SAADC_DEFAULT_CONFIG;
#define SAMPLES_IN_BUFFER 1
#define SAMPLES_BUFFER 4
static nrf_saadc_value_t buffer_pool[2][SAMPLES_BUFFER];

// record battery state
uint8_t percentage = 255;

static uint8_t trans_persentage(uint16_t voltage)
{
    if (voltage >= 4100)
        percentage = 100;
    else if (voltage >= 3335)
        percentage = 15 + (voltage - 3335) / 9;
    else if (voltage >= 2900)
        percentage = (voltage - 2900) / 29;
    return percentage;
}

static void adc_result_handler(nrf_saadc_value_t value)
{
        
        // RESULT = [V(P) – V(N) ] * GAIN/REFERENCE * 2 ^ (RESOLUTION - m)
        // value  = V_in / 1.2 * 1024
        // V_in   = V_bat * 2.2 / 12.2
        uint16_t voltage = value * 1200 * 122 / 1024 / 22;
        trans_persentage(voltage);
        // ble upate
        // todo 
        kb_nrf_print("voltage %d percent %d%%", voltage, percentage);
}

void adc_evt_cb(nrfx_saadc_evt_t const* event)
{
    nrf_saadc_value_t results[1] = { 0 };

    if (event->type != NRFX_SAADC_EVT_DONE) {
      return;
    }

    ret_code_t err_code;
    // reload Buffer
    err_code = nrfx_saadc_buffer_convert(event->data.done.p_buffer, SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    // get average result
    for (uint8_t j = 0; j < SAMPLES_IN_BUFFER; j++) {
        results[j] += event->data.done.p_buffer[j];
    }
    results[0];

    adc_result_handler(results[0]);

    nrfx_saadc_uninit();
    saadc_initialized = false;
}

/* init adc to mesure voltage */
static void saadc_init()
{
    ret_code_t err_code;

    err_code = nrfx_saadc_init(&config, adc_evt_cb);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_channel_init(0, &kb_battery_channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_buffer_convert(buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_buffer_convert(buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);
}

void battery_level_meas_timeout_handler(void * p_context)
{
    kb_nrf_print("measure ");
  // forbid reenter
    bool measure_started = true;



    if (measure_started) {
        if (!saadc_initialized) {
            saadc_initialized = !saadc_initialized;
            saadc_init();
        }
        // sampling
        measure_started = !measure_started;
        nrfx_err_t err_code = nrfx_saadc_sample();
        APP_ERROR_CHECK(err_code);
    }
}

APP_TIMER_DEF(m_battery_timer_id);                                  /**< Battery timer. */

void kb_power_mgr_init()
{
    ret_code_t err_code;
    // Create battery timer.
    err_code = app_timer_create(&m_battery_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                battery_level_meas_timeout_handler);
    APP_ERROR_CHECK(err_code);
}

void kb_power_mgr_start()
{
    ret_code_t err_code;
    err_code = app_timer_start(m_battery_timer_id, MEASURE_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);
    kb_nrf_print("pwr mgr start");
}