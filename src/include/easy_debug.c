/* Copyright (C) 2021 Bas Brugman
 * http://www.visionnaire.nl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "easy_debug.h"

#include "easy_usart.h"
#include "stdio.h"
#include "string.h"

void console_setup(void);

// setup usart2 for terminal debugging
void debug_setup() { usart2_setup(); }

void debug_write(char* str) { usart2_send_str(str); }

// setup little console debug menu
// which can be used to send opcodes that trigger all events
// via usart.
void console_setup() {}

// if NDEBUG is defined, all assertion checks are disabled and macros do not
// generate any code.
//
// Use these checks: cm3_assert(expr), cm3_assert_not_reached()
//
// if CM3_ASSERT_VERBOSE is defined, information regarding the position of
// assertion checks will be stored in the binary, allowing for more
// informative error messages, but also significantly increased code size.
void cm3_assert_failed(void) {
  debug_write("assert failed!\n");
  while (1)
    ;
}

// write extra assert info to the debug console
void cm3_assert_failed_verbose(const char* file, int line, const char* func,
                               const char* assert_expr) {
  char buf[255];
  snprintf(buf, 255, "file: %s\nline: %d\nfunc: %s\nexpr %s\n", file, line,
           func, assert_expr);
  debug_write(buf);
  cm3_assert_failed();
}

