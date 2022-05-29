#include "nrf.h"
#include "timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "app_timer.h"

#define TIMER_TICKS(s, pre) APP_TIMER_TICKS(s)

#define TICK_INTERVAL 10
/* Mill second tick count */
volatile uint32_t timer_count = 0;

APP_TIMER_DEF(one_ms_ticker_id);                          /**< 1ms timer. */

void timer_tick(uint8_t interval) {
  timer_count += interval;
}
/* Timer interrupt handler */
void ticker(void* context)  {
    timer_count+=TICK_INTERVAL;
    app_timer_create(&one_ms_ticker_id, APP_TIMER_MODE_SINGLE_SHOT, ticker);
    app_timer_start(one_ms_ticker_id, TIMER_TICKS(TICK_INTERVAL, 0), NULL);
}

/* Timer Initialize
 * Using single shot timer because task queue will overflow
 * if the timer runs in repeated mode during bonding
 */
void timer_init(void)
{
  uint32_t err_code = app_timer_create(&one_ms_ticker_id, APP_TIMER_MODE_SINGLE_SHOT, ticker);
//  err_code = app_timer_start(one_ms_ticker_id, TIMER_TICKS(TICK_INTERVAL, 0), NULL);
  APP_ERROR_CHECK(err_code);
}

void timer_clear(void)
{
    timer_count = 0;
}

uint16_t timer_read(void)
{
    return (uint16_t)(timer_count & 0xFFFF);
}

uint32_t timer_read32(void)
{
    return timer_count;
}

uint16_t timer_elapsed(uint16_t last)
{
    return TIMER_DIFF_16(timer_read(), last);
}

uint32_t timer_elapsed32(uint32_t last)
{
    return TIMER_DIFF_32(timer_read32(), last);
}
