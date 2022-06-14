// ssd1306.c
#include "nrf_delay.h"
#include "i2c_master.h"
#include <stdint.h>
#include <string.h>

// 0111100 和 0b111101
#define SSD1306ADDR 0b111100
#if PIXEL_HEIGHT == 32
#define upattern_t uint32_t
#else
#define upattern_t uint64_t
#endif

#define PIXEL_WIDTH 128
#define PIXEL_HEIGHT 32
#define COL_COUNT 128
#define ROW_COUNT PIXEL_HEIGHT / 8
bool dirty = false;

#ifndef CONFIG_PIN_SCL
#define CONFIG_PIN_SCL 3
#endif
#ifndef CONFIG_PIN_SDA
#define CONFIG_PIN_SDA 4
#endif

uint8_t data[ROW_COUNT*COL_COUNT+1];

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
// uint8_t data[COL_COUNT+1];

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



void ssd1306_init(void)
{
    i2c_init(CONFIG_PIN_SDA, CONFIG_PIN_SCL);

    nrf_delay_ms(100);

    write_i(0xAE); /*display off*/

    write_i(0x00); /*set lower column address*/
    write_i(0x10); /*set higher column address*/
    write_i(0x00); /*set display start line*/
    write_i(0xB0);  /*set page address*/

    write_i(0x81); /*contract control*/
    write_i(0xf0);  /*128*/
    
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
    write_i3(0x22, 0, ROW_COUNT - 1);

    write_i(0xAF); /*display ON*/


    nrf_delay_ms(100);

    clear();

    render();
}



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

upattern_t render_matrix[COL_COUNT];

void __rect_core(uint8_t border_width)
{
    for (size_t i = 0; i < COL_COUNT; i++)
    {
        if (i < border_width || i > COL_COUNT - 1 - border_width)
        {
            render_matrix[i] = 0xFFFFFFFF;
        } else {
            render_matrix[i] = (0xFFFFFFFF >> (PIXEL_HEIGHT - border_width)) | (0xFFFFFFFF << (PIXEL_HEIGHT - border_width));
        }
    }
}
void rectv2(uint8_t border_width)
{

    __rect_core(border_width);

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
    __rect_core(border_width);
    dirty = true;
    if (left_offset == 0)
    {
        return;
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
}

struct rect
{
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
    bool solid;
    uint8_t border;
};

void rectv4(struct rect *rect)
{  
    for (size_t i = 0; i < COL_COUNT; i++)
    { 
        if (i >= rect->x && i < rect->x + rect->w) {
            render_matrix[i] = (0xFFFFFFFF << rect->y) & (0xFFFFFFFF >> (MAX(0, PIXEL_HEIGHT - rect->y - rect->h)));
        } else {
            render_matrix[i] = 0x0;
        }
    }

    if (!rect->solid && rect->border)
    {
        for (size_t i = 0; i < COL_COUNT; i++)
        {
            if (i >= rect->x + rect->border && 
                i < rect->x + rect->w - rect->border) {
                render_matrix[i] ^= (0xFFFFFFFF << (rect->y + rect->border)) & (0xFFFFFFFF >> MAX(PIXEL_HEIGHT - rect->y - rect->h + rect->border, 0));
                // render_matrix[i] ^= (0xFFFFFFFF << 1) & (0xFFFFFFFF >> 21;
            }
        }
    }
    
    dirty = true;
    
    for (size_t i = 0; i < ROW_COUNT; i++)
    {
        for (size_t j = 0; j < COL_COUNT; j++)
        {
            data[i * COL_COUNT + j+ 1] = render_matrix[j] >> (i * 8);
        }
    }
}

 void picture(uint8_t *pic)
 {
    for (size_t j = 0; j < PIXEL_HEIGHT; j++) {        
        for (size_t i = 0; i < PIXEL_WIDTH; i++){
            uint16_t pixel_idx = i + j * PIXEL_WIDTH;
            uint16_t target_idx = i + (j / 8) * PIXEL_WIDTH;
            uint16_t pic_idx = pixel_idx / 8;
            
            uint8_t target_bit = j % 8;
            uint8_t pic_bit = i % 8;
            uint8_t result = pic[pic_idx];
            data[target_idx + 1] |= ((result >> pic_bit & 1) << target_bit);
        }
    }
     dirty = true;
 }
