// ssd1306.h

#pragma once
#include <stdint.h>
void ssd1306_init(void);

void write_i(uint8_t data);

void write_i2(uint8_t cmd, uint8_t data2);

void write_i3(uint8_t cmd, uint8_t data2, uint8_t data3);

void clear(void);

void render(void);

 void picture(uint8_t *pic);