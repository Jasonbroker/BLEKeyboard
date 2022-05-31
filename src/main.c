
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

#define PIXEL_WIDTH 128
#define PIXEL_HEIGHT 32
#define COL_COUNT 128
#define ROW_COUNT PIXEL_HEIGHT / 8

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
    // uint8_t *real_matrix = (uint8_t *)(&data + 1);
    for (size_t i = 1; i < sizeof(data); i++)
    {
        // row
        uint8_t row = (i - 1) / COL_COUNT;
        uint8_t col = (i - 1) % COL_COUNT;

        if (col < border_width)
        {
            data[i] = 0xff;
        } else if (col >= COL_COUNT - border_width) {
            data[i] = 0xff;
        } else {
            if (row == 0)
            {
                data[i] = 0xff >> (8 - border_width);
            } else if (row == ROW_COUNT - 1) {
                data[i] = 0xff << (8 - border_width);
            }
        }
    }
}

void rectv2(uint8_t border_width)
{
    // 128*32
    for (size_t j = 1; j < sizeof(data); j++)
    {
        
    }
}

void clear(void)
{
    memset(data, 0x00, sizeof(data));
}

void render(void)
{
    // write_i(0xB0);  /*set page address*/
    data[0] = 0x40;
    i2c_transmit(SSD1306ADDR, data, sizeof(data), false);
}

void test(void)
{
    i2c_init();

    nrf_delay_ms(100);
    
    ssd1306_init();

    nrf_delay_ms(100);

    // clear
    clear();

    rect(1);

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

