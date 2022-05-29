
#pragma once
#include <stdint.h>
#include <stdbool.h>

int i2c_init(void);
void i2c_uninit(void);
uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length, bool no_stop);