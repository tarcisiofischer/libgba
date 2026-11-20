#ifndef _LIBGBA_TXT_H
#define _LIBGBA_TXT_H

#include <libgba/types.h>

// Initialize text system: configures Mode 0, BG0, charblock 0, screenblock 8,
// loads the bundled font tiles & palette, and clears the screen.
void txt_init();

// Clear the text screen and reset cursor to (0, 0).
void clear_screen();

// Move cursor to specific tile position. (0 <= col < 30, 0 <= row < 20)
void set_cursor(int col, int row);

// Absolute-position character and string rendering.
void put_char(int col, int row, char c);
void draw_text(int col, int row, const char* str);

// Terminal-like streaming output (auto-wrap at col 30, scrolls up at row 20).
void write(char c);
void write(const char* str);
void write_int(int val);

inline void write_char(char c) {
    write(c);
}

#endif // _LIBGBA_TXT_H
