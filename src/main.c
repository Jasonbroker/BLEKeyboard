
#include "print.h"
#include "timer.h"

// add timer
#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "ssd1306.h"
#include "animation_frame.h"

uint8_t i = 0;
void tick(void* context)
{
    /*
    static int8_t direction = 1;
    if (i == 16)
    {
       direction = -1;
    } else if (i == 1) {
        direction = 1;
    }
    i = i + direction;
    struct rect r = {
        .x = 0,
        .y = 0,
        .w = 128,
        .h = 32,
        .solid = false,
        .border = i,
    };
    rectv4(&r);
    */
   if (i == 6)
   {
       i = 0;
   }
   clear();   
    picture(frame_data(i));
    i++;

    render();    
}

#define FRESH_RATE 20

APP_TIMER_DEF(m_ticker_id);

void add_render_callback(void) 
{
    ret_code_t ret;
    ret = nrf_drv_clock_init();
    APP_ERROR_CHECK(ret);

    nrf_drv_clock_lfclk_request(NULL);
    while(!nrf_drv_clock_lfclk_is_running())
    {
        /* Just waiting */
    }
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_create(&m_ticker_id, APP_TIMER_MODE_REPEATED, tick);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_ticker_id, APP_TIMER_TICKS(1000/FRESH_RATE), NULL);
    APP_ERROR_CHECK(err_code);
}

#include "app_scheduler.h"
#include "nrf_pwr_mgmt.h"


int main(int argc, char const *argv[])
{
    log_init();
    
    printf("init done");

    ssd1306_init();

    add_render_callback();
    
    for (;;) {
#ifdef APP_TIMER_CONFIG_USE_SCHEDULER
        app_sched_execute();
#endif
        if (NRF_LOG_PROCESS() == false) {
        nrf_pwr_mgmt_run();
        }
    }
}

