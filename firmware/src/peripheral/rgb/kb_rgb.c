#include "kb_rgb.h"
#include "i2c_master.h"
#include "issi/is31fl3741.h"
#include "config.h"
#include "kb_nrf_print.h"
#include "app_timer.h"

void timeout_handler()
{
    kb_nrf_print("twi test send");

    device_detector();
}

APP_TIMER_DEF(m_timer_id);     
void common_test()
{
    ret_code_t err_code;
    // Create battery timer.
    err_code = app_timer_create(&m_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_timer_id, APP_TIMER_TICKS(3000), NULL);
    APP_ERROR_CHECK(err_code);

    kb_nrf_print("common_test start");

}

///////////

static void flush(void) { IS31FL3741_update_pwm_buffers(DRIVER_ADDR_1, DRIVER_ADDR_2); }

typedef struct {
    /* Perform any initialisation required for the other driver functions to work. */
    void (*init)(void);
    /* Set the colour of a single LED in the buffer. */
    void (*set_color)(int index, uint8_t r, uint8_t g, uint8_t b);
    /* Set the colour of all LEDS on the keyboard in the buffer. */
    void (*set_color_all)(uint8_t r, uint8_t g, uint8_t b);
    /* Flush any buffered changes to the hardware. */
    void (*flush)(void);
} rgb_matrix_driver_t;

void init();

const rgb_matrix_driver_t rgb_matrix_driver = {
    .init = init,
    .flush = flush,
    .set_color = IS31FL3741_set_color,
    .set_color_all = IS31FL3741_set_color_all,
};


static nrfx_twi_t* twi_channel;

void init()
{
    i2c_init();
    i2c_start(DRIVER_ADDR_1);
    IS31FL3741_init(DRIVER_ADDR_1);

    for (int index = 0; index < DRIVER_LED_TOTAL; index++) {
        bool enabled = true;
      // This only caches it for later
        IS31FL3741_set_led_control_register(index, enabled, enabled, enabled);
    }

    IS31FL3741_update_led_control_registers(DRIVER_ADDR_1, 0);
}
