#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "nrfx_twi.h"

#ifdef I2C1_BANK
#    define I2C1_SCL_BANK I2C1_BANK
#    define I2C1_SDA_BANK I2C1_BANK
#endif

#ifndef I2C1_SCL_BANK
#    define I2C1_SCL_BANK GPIOB
#endif

#ifndef I2C1_SDA_BANK
#    define I2C1_SDA_BANK GPIOB
#endif

#ifndef I2C1_SCL
#    define I2C1_SCL 4
#endif
#ifndef I2C1_SDA
#    define I2C1_SDA 3
#endif

#ifdef USE_I2CV1
#    ifndef I2C1_OPMODE
#        define I2C1_OPMODE OPMODE_I2C
#    endif
#    ifndef I2C1_CLOCK_SPEED
#        define I2C1_CLOCK_SPEED 100000 /* 400000 */
#    endif
#    ifndef I2C1_DUTY_CYCLE
#        define I2C1_DUTY_CYCLE STD_DUTY_CYCLE /* FAST_DUTY_CYCLE_2 */
#    endif
#else
// The default timing values below configures the I2C clock to 400khz assuming a 72Mhz clock
// For more info : https://www.st.com/en/embedded-software/stsw-stm32126.html
#    ifndef I2C1_TIMINGR_PRESC
#        define I2C1_TIMINGR_PRESC 0U
#    endif
#    ifndef I2C1_TIMINGR_SCLDEL
#        define I2C1_TIMINGR_SCLDEL 7U
#    endif
#    ifndef I2C1_TIMINGR_SDADEL
#        define I2C1_TIMINGR_SDADEL 0U
#    endif
#    ifndef I2C1_TIMINGR_SCLH
#        define I2C1_TIMINGR_SCLH 38U
#    endif
#    ifndef I2C1_TIMINGR_SCLL
#        define I2C1_TIMINGR_SCLL 129U
#    endif
#endif

//#ifndef I2C_DRIVER
//#    define I2C_DRIVER I2CD1
//#endif

#ifdef USE_GPIOV1
#    ifndef I2C1_SCL_PAL_MODE
#        define I2C1_SCL_PAL_MODE PAL_MODE_STM32_ALTERNATE_OPENDRAIN
#    endif
#    ifndef I2C1_SDA_PAL_MODE
#        define I2C1_SDA_PAL_MODE PAL_MODE_STM32_ALTERNATE_OPENDRAIN
#    endif
#else
// The default PAL alternate modes are used to signal that the pins are used for I2C
#    ifndef I2C1_SCL_PAL_MODE
#        define I2C1_SCL_PAL_MODE 4
#    endif
#    ifndef I2C1_SDA_PAL_MODE
#        define I2C1_SDA_PAL_MODE 4
#    endif
#endif

typedef int16_t i2c_status_t;

#define I2C_STATUS_SUCCESS (0)
#define I2C_STATUS_ERROR (-1)
#define I2C_STATUS_TIMEOUT (-2)

void         i2c_init(void);
i2c_status_t i2c_start(uint8_t address);
i2c_status_t i2c_transmit(uint8_t address, const uint8_t* data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t* data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout);
void         i2c_stop(void);
// use for debug
void device_detector();