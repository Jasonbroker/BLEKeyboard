/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"
#include "nrf_gpio.h"
#include "wait.h"
#include "config.h"

#ifdef KB_NRF_DEBUG
  #include "kb_nrf_print.h"
#endif

/*
 * scan matrix
 */


#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static void select_row(uint8_t row);
static matrix_row_t read_cols(void);
static void unselect_row(uint8_t row);
static void unselect_rows(void);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

#define LED_ON()    do { /*palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)
#define LED_OFF()   do { /*palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)
#define LED_TGL()   do { /*palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)


/* Column pin configuration
 */
static void  init_cols(void)
{
    for (uint8_t i = 0;  i < MATRIX_COLS; i++) {
        nrf_gpio_cfg_input((uint32_t)col_pins[i], NRF_GPIO_PIN_PULLUP);
    }
}

void matrix_init(void)
{
  #ifdef KB_NRF_DEBUG
    kb_nrf_print("matrix init");
  #endif 
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();

        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                //debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_row(i);
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}



/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(void)
{
    uint32_t result = 0;
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        if (!nrf_gpio_pin_read(((uint32_t)col_pins[i])))
            result |= 1 << i;
    }

    return result;
}

/* Row pin configuration
 */
static void unselect_row(uint8_t row)
{
   nrf_gpio_pin_set((uint32_t)row_pins[row]);
}

static void unselect_rows(void)
{
  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    nrf_gpio_pin_set((uint32_t)row_pins[i]);
  }
}

static void select_row(uint8_t row)
{
    nrf_gpio_cfg((uint32_t)row_pins[row], NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_pin_clear((uint32_t)row_pins[row]);
}

void matrix_print(void)
{
    #ifdef KB_NRF_DEBUG
      kb_nrf_print("matrix changed");
    #endif 
}
