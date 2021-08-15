#include "kb_rgb.h"
#include "i2c_master.h"
#include "issi/is31fl3741.h"
#include "config.h"
#include "kb_nrf_print.h"
#include "app_timer.h"
#include "rgb_matrix.h"

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



void init();

const rgb_matrix_driver_t rgb_matrix_driver = {
    .init = init,
    .flush = flush,
    .set_color = IS31FL3741_set_color,
    .set_color_all = IS31FL3741_set_color_all,
};


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

void rgb_matrix_init()
{
      rgb_matrix_driver.init();
}

void rgb_matrix_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_driver.set_color(index, red, green, blue);
}

void rgb_matrix_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_driver.set_color_all(red, green, blue);
}

void rgb_matrix_update_pwm_buffers(void) { rgb_matrix_driver.flush(); }

void rgb_matrix_test(void) {
  rgb_matrix_set_color_all(20, 20, 20);
  rgb_matrix_update_pwm_buffers();
    // Mask out bits 4 and 5
    // Increase the factor to make the test animation slower (and reduce to make it faster)
    //uint8_t factor = 10;
    //switch ((g_rgb_timer & (0b11 << factor)) >> factor) {
    //    case 0: {
    //        rgb_matrix_set_color_all(20, 0, 0);
    //        break;
    //    }
    //    case 1: {
    //        rgb_matrix_set_color_all(0, 20, 0);
    //        break;
    //    }
    //    case 2: {
    //        rgb_matrix_set_color_all(0, 0, 20);
    //        break;
    //    }
    //    case 3: {
    //        rgb_matrix_set_color_all(20, 20, 20);
    //        break;
    //    }
    //}
}