#include "timer.h"
#include <stdint.h>

#include "app_timer.h"

#define TICK_PER_MILLISECOND (32768 / (APP_TIMER_CONFIG_RTC_FREQUENCY + 1) / 1000)

void timer_init() {}

void timer_clear() {}

inline uint16_t timer_read()
{
    return (uint16_t)(timer_read32() % 0xFFFF);
}

inline uint32_t timer_read32()
{
    uint32_t time;
    // 24bit, 32768 tick per sec.
    // so only 19 bits are valid.

    // but, tmk does not use 32bit timer
    time = app_timer_cnt_get();
    return time / TICK_PER_MILLISECOND;
}

inline uint16_t timer_elapsed(uint16_t last)
{
    return TIMER_DIFF_16(timer_read(), last);
}

inline uint32_t timer_elapsed32(uint32_t last)
{
    uint32_t time = app_timer_cnt_get();
    uint32_t elapsed = app_timer_cnt_diff_compute(time, last * TICK_PER_MILLISECOND);
    return elapsed / TICK_PER_MILLISECOND;
}