
#include "print.h"
#include "ssd1306.h"
#include "i2c_master.h"
#include "timer.h"
#include "nrf_delay.h"

// 0111100 和 0b111101
#define SSD1306ADDR 0b111100

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

    write_i(0xAF); /*display ON*/
}

#define PIXEL_WIDTH 128
#define PIXEL_HEIGHT 32

uint8_t data[PIXEL_WIDTH*PIXEL_HEIGHT/8+1];
void test(void)
{
    i2c_init();

    nrf_delay_ms(100);
    
    ssd1306_init();

    nrf_delay_ms(100);

    // clear
    // write_i3(0x22, 0, 0);

    // uint8_t data[PIXEL_WIDTH + 1];
    // memset(data, 0x00, sizeof(data));

    data[0] = 0x40;

    for (size_t i = 1; i < PIXEL_WIDTH + 1; i++)
    {
        if (i < 3)
        {
            data[i] = 0xff;
        } else if (i >= PIXEL_WIDTH) {
            data[i] = 0xff;
        } else {
            data[i] = 0x0f;
        }
        data[i] = 0x00;
    }

    // write_i3(0x21, 0, PIXEL_WIDTH);

    i2c_transmit(SSD1306ADDR, data, PIXEL_WIDTH + 1, false);

        i2c_transmit(SSD1306ADDR, data, PIXEL_WIDTH + 1, false);

    // i2c_transmit(SSD1306ADDR, data, PIXEL_WIDTH + 1, false);

    // i2c_transmit(SSD1306ADDR, data, PIXEL_WIDTH + 1, false);
    // i2c_transmit(SSD1306ADDR, data, PIXEL_HEIGHT + 1, false);
    // i2c_transmit(SSD1306ADDR, data, 128 + 1, false);
    for (;;) {
        NRF_LOG_PROCESS();
    }
}

// 0-127 0 - 3
void move_cursor_2_pos(uint8_t x, uint8_t y)
{

}

void timer_tick(uint8_t interval);
void final(void);
int main(int argc, char const *argv[])
{
    log_init();
    
    printf("init done");
    dprintf("init debug done");

    test();
    // final();
}


void final(void)
{
    timer_init();

    iota_gfx_init(false);

    iota_gfx_write_char('a');
    for (;;) {
        NRF_LOG_PROCESS();
        iota_gfx_task();
        timer_tick(1);
    }
}

