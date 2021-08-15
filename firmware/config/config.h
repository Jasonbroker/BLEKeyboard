/*
Copyright 2015 Jun Wako <wakojun@gmail.com>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdint.h"

#ifndef CONFIG_H
#define CONFIG_H

#define KB_NRF_DEBUG

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6464
#define DEVICE_VER      0x0001
#define MANUFACTURER    TMK
#define PRODUCT         RCRD
#define DESCRIPTION     RCRD keyboard firmware for template

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    1

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

#define BACKLIGHT_LEVELS 3
#define BATTERY_PIN NRF_SAADC_INPUT_AIN0

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 15
static const uint8_t row_pins[MATRIX_ROWS] = {12, 7, 8, 9, 10, 11};
static const uint8_t col_pins[MATRIX_COLS] = {29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 16, 15, 14, 13};
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
#define NO_DEBUG

/* disable print */
#define NO_PRINT

// temp comment
/* disable action features */
#define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define  RGB_MATRIX_ENABLE
// rgb 
#ifdef RGB_MATRIX_ENABLE
// #    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
// #    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_RAINDROPS

// #    define DEBUG_MATRIX_SCAN_RATE

#    define RGB_MATRIX_LED_FLUSH_LIMIT 100
// #    define RGB_MATRIX_LED_PROCESS_LIMIT 2

// i2c_master defines
#    define I2C_COUNT 1

#    define I2C1_BANK GPIOB
#    define I2C1_SCL 4 // A2 on pinout = B0
#    define I2C1_SDA 3 // A2 on pinout = B1
#    define I2C1_SCL_PAL_MODE PAL_MODE_ALTERNATIVE_2
#    define I2C1_SDA_PAL_MODE PAL_MODE_ALTERNATIVE_2

#    define DRIVER_ADDR_1 (0x30>>1)
#    define DRIVER_ADDR_2 DRIVER_ADDR_1
#    define DRIVER_COUNT 1

// led defines
#    define DRIVER_1_LED_TOTAL 68
#    define DRIVER_2_LED_TOTAL 0
#    define DRIVER_LED_TOTAL (DRIVER_1_LED_TOTAL)
#define DRIVER_INDICATOR_LED_TOTAL 0
#endif
#endif
