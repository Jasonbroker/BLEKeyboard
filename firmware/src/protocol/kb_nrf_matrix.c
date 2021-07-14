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
#ifdef CONFIG_HEADER
#include CONFIG_HEADER
#endif

static matrix_row_t matrix[MATRIX_ROWS];

/* should be called at early stage of startup before matrix_init.(optional) */
//void matrix_setup(void)
//{

//}
///* intialize matrix for scaning. */
//void matrix_init(void)
//{

//}
///* scan all key states on matrix */
//uint8_t matrix_scan(void)
//{

//}

///* matrix state on row */
//matrix_row_t matrix_get_row(uint8_t row)
//{
//    return matrix[row];
//{
///* print matrix for debug */
//void matrix_print(void)
//{
//}
///* clear matrix */
//void matrix_clear(void)
//{
//}

