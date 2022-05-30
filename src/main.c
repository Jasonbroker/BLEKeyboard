
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
    write_i(0x80);  /*128*/
    
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

    write_i(0xAF); /*display ON*/
}

#define PIXEL_WIDTH 128
#define PIXEL_HEIGHT 32
#define COL_COUNT 128
#define ROW_COUNT PIXEL_HEIGHT / 8

uint8_t data[ROW_COUNT][COL_COUNT + 1];

uint8_t pattern[] = {0xfe, 0xff, 0x07};
void border(void)
{
    for (size_t j = 0; j < ROW_COUNT; j++)
    {
        for (size_t i = 1; i < COL_COUNT+1; i++)
        {
            if (j == 0) // 第一行
            {
                if (i <= 3 ) {
                    data[j][i] = pattern[i-1];
                } else if (i > COL_COUNT - 3) {
                    data[j][i] = pattern[COL_COUNT - i];
                } else {
                    data[j][i] = 0x03;    
                }
            } else if (j == ROW_COUNT - 1) { // last row
                if (i == 1 || i == 2)
                {
                    data[j][i] = 0xff;    
                } else if (i > COL_COUNT - 2)
                {
                    data[j][i] = 0xff;
                } else {
                    data[j][i] = 0xc0;    
                }
            } else {

                if (i == 1 || i == 2)
                {
                    data[j][i] = 0xff;    
                } else if (i > COL_COUNT - 2)
                {
                    data[j][i] = 0xff;
                } else {
                    data[j][i] = 0x00;    
                }
            }
        }
    }
}

void render(void)
{
    for (size_t i = 0; i < ROW_COUNT; i++)
    {
        // write_i(0xB0+i);
        data[i][0] = 0x40;
        i2c_transmit(SSD1306ADDR, data[i], sizeof(data[i]), false);
    }
}

void test(void)
{
    i2c_init();

    nrf_delay_ms(100);
    
    ssd1306_init();

    nrf_delay_ms(100);

    // clear
    // write_i3(0x21, 0, 127);
    // write_i3(0x22, 0, 7);

    border();

    render();

    // write_i3(0x21, 0, PIXEL_WIDTH);
    for (;;) {
        NRF_LOG_PROCESS();
    }
}

void timer_tick(uint8_t interval);
void final(void);
int main(int argc, char const *argv[])
{
    log_init();
    
    printf("init done");

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

