
#include "print.h"
#include "i2c_master.h"
#include "timer.h"
#include "nrf_delay.h"

// 0111100 和 0b111101
#define SSD1306ADDR 0b111100
#define FRESH_RATE 20

inline static void __write_i(uint8_t cmd, bool no_stop)
{
    uint8_t data[] = {0x00, cmd};
    i2c_transmit(SSD1306ADDR, data, 2, no_stop);
}

void write_i(uint8_t data)
{
    __write_i(data, false);
}

void write_i2(uint8_t cmd, uint8_t data2)
{
    uint8_t data[] = {0x00, cmd, data2};
    i2c_transmit(SSD1306ADDR, data, 3, false);
}

void write_i3(uint8_t cmd, uint8_t data2, uint8_t data3)
{
    uint8_t data[] = {0x00, cmd, data2, data3};
    i2c_transmit(SSD1306ADDR, data, 4, false);
}

#define PIXEL_WIDTH 128
#define PIXEL_HEIGHT 32
#define COL_COUNT 128
#define ROW_COUNT PIXEL_HEIGHT / 8
bool dirty = false;

void ssd1306_init(void)
{
    write_i(0xAE); /*display off*/

    write_i(0x00); /*set lower column address*/
    write_i(0x10); /*set higher column address*/
    write_i(0x00); /*set display start line*/
    write_i(0xB0);  /*set page address*/

    write_i(0x81); /*contract control*/
    write_i(0x00);  /*128*/
    
    write_i(0xA1); /*set segment remap*/

    write_i(0xA6); /*normal / reverse*/

    write_i(0xA8); /*multiplex ratio*/
    write_i(0x1F); /*duty = 1/32*/

    write_i(0xC8); /*Com scan direction*/

    write_i(0xD3); /*set display offset*/
    write_i(0x00);

    write_i(0xD5); /*set osc division*/
    write_i(0x80);

    write_i(0xD9); /*set pre-charge period*/
    write_i(0x1f);

    write_i(0xDA); /*set COM pins*/
    write_i(0x00);

    write_i(0xdb); /*set vcomh*/
    write_i(0x40); 

    write_i(0x8d); /*set charge pump enable*/
    write_i(0x14); 
    write_i2(0x20, 0); // horizonal addressing mode
    write_i3(0x21, 0, PIXEL_WIDTH - 1);
    write_i3(0x22, 0, 3);

    write_i(0xAF); /*display ON*/
}

uint8_t data[ROW_COUNT*COL_COUNT+1];
// uint8_t data[COL_COUNT+1];

uint8_t pattern[] = {0xfe, 0xf0, 0x07};

void rect(uint8_t border_width)
{
    // 128*32
    for (size_t i = 1; i < sizeof(data); i++)
    {
        // row
        uint8_t row = (i - 1) / COL_COUNT;
        uint8_t col = (i - 1) % COL_COUNT;

        if (col < border_width || col >= COL_COUNT - border_width)
        {
            data[i] = 0xff;
        } else {
            if (row == 0)
            {
                data[i] = 0xff >> (8 - border_width);
            } else if (row == ROW_COUNT - 1) {
                data[i] = 0xff << (8 - border_width);
            } else {
                data[i] = 0x00;
            }
        }
    }
    dirty = true;
}

uint32_t render_matrix[COL_COUNT];
void rectv2(uint8_t border_width)
{

    for (size_t i = 0; i < COL_COUNT; i++)
    {
        if (i < border_width || i > COL_COUNT - 1 - border_width)
        {
            render_matrix[i] = 0xFFFFFFFF;
        } else {
            render_matrix[i] = (0xFFFFFFFF >> (32 - border_width)) | (0xFFFFFFFF << (32 - border_width));
        }
    }

    for (size_t i = 0; i < ROW_COUNT; i++)
    {
        for (size_t j = 0; j < COL_COUNT; j++)
        {
            data[i * COL_COUNT + j+ 1] = render_matrix[j] >> (i * 8);
        }
    }
    dirty = true;
}

void rectv3(uint8_t border_width, uint8_t left_offset)
{  
    for (size_t i = 0; i < COL_COUNT; i++)
    {
        if (i < border_width || i > COL_COUNT - 1 - border_width)
        {
            render_matrix[i] = 0xFFFFFFFF;
        } else {
            render_matrix[i] = (0xFFFFFFFF >> (32 - border_width)) | (0xFFFFFFFF << (32 - border_width));
        }
    }

    for (size_t i = 0; i < ROW_COUNT; i++)
    {
        for (size_t j = 0; j < COL_COUNT; j++)
        {
            if (j < left_offset)
            {
                data[i * COL_COUNT + j+ 1] = 0;
            } else {
                data[i * COL_COUNT + j+ 1] = render_matrix[j - left_offset] >> (i * 8);
            }
            
            
        }
    }

    dirty = true;
}

void clear(void)
{
    memset(data, 0x00, sizeof(data));
    dirty = true;
}

void render(void)
{
    if (!dirty)
    {
        return;
    }
    dirty = false;
    data[0] = 0x40;
    i2c_transmit(SSD1306ADDR, data, sizeof(data), false);
}

// add timer
#include "app_timer.h"
#include "nrf_drv_clock.h"

uint8_t i = 1;
void tick(void* context)
{
    static int8_t direction = 1;
    if (i == 16)
    {
       direction = -1;
    } else if (i == 1) {
        direction = 1;
    }
    i = i + direction;
    rectv3(i, 64);
    render();    
}

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

#ifndef CONFIG_PIN_SCL
#define CONFIG_PIN_SCL 3
#endif
#ifndef CONFIG_PIN_SDA
#define CONFIG_PIN_SDA 4
#endif



int main(int argc, char const *argv[])
{
    log_init();
    
    printf("init done");

    i2c_init(CONFIG_PIN_SDA, CONFIG_PIN_SCL);

    nrf_delay_ms(100);
    
    ssd1306_init();

    nrf_delay_ms(100);

    clear();

    render();

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

