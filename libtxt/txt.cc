#include <libgba/gba.h>
#include <libgba/types.h>
#include <libgba/txt.h>
#include "font_tileset.inc"

static volatile u16_t* const TILESET_VRAM = reinterpret_cast<volatile u16_t*>(0x06000000);
static volatile u16_t* const TILEMAP_VRAM = reinterpret_cast<volatile u16_t*>(0x06000000 + 8 * 0x800);
static constexpr int TILEMAP_COLS = 30;  // GBA screen: 240px / 8px = 30 tiles wide
static constexpr int TILEMAP_ROWS = 20;  // GBA screen: 160px / 8px = 20 tiles tall
static constexpr u16_t VRAM_FONT_TILE_BASE = 1;

static int cursor_col = 0;
static int cursor_row = 0;

static void load_font() {
    BG_PALETTE[0] = FONT_PALETTE[0];  // 0x0000 black (transparent)
    BG_PALETTE[1] = FONT_PALETTE[1];  // 0x7FFF white

    constexpr int WORDS_PER_TILE = 16;
    for (u16_t i = 0; i < FONT_NUM_CHARS; ++i) {
        const u16_t* src = FONT_TILESET + i * WORDS_PER_TILE;
        volatile u16_t* dst = TILESET_VRAM + (VRAM_FONT_TILE_BASE + i) * WORDS_PER_TILE;
        for (int w = 0; w < WORDS_PER_TILE; ++w) {
            dst[w] = src[w];
        }
    }
}

static void setup_display() {
    *REG_DISPCNT = MODE_0 | BG0_ENABLE;
    *REG_BG0CNT =
        (0  << 0)  |   // priority 0 (highest)
        (0  << 2)  |   // charblock 0 (0x06000000)
        (0  << 6)  |   // mosaic off
        (0  << 7)  |   // 4bpp
        (8  << 8)  |   // screenblock 8 (0x06004000)
        (0  << 13) |   // screen-over (unused in mode 0)
        (0  << 14);    // 32x32 tiles

    *REG_BG0HOFS = 0;
    *REG_BG0VOFS = 0;
}

void txt_init() {
    setup_display();
    load_font();
    clear_screen();
}

void clear_screen() {
    for (int i = 0; i < 32 * 32; ++i) {
        TILEMAP_VRAM[i] = 0;
    }
    cursor_col = 0;
    cursor_row = 0;
}

void set_cursor(int col, int row) {
    cursor_col = col;
    cursor_row = row;
}

void put_char(int col, int row, char c) {
    if (col < 0 || col >= TILEMAP_COLS || row < 0 || row >= TILEMAP_ROWS) return;
    int ascii = static_cast<int>(static_cast<unsigned char>(c));
    int tile_index = ascii - FONT_ASCII_START;
    if (tile_index < 0 || tile_index >= FONT_NUM_CHARS) return;
    TILEMAP_VRAM[row * 32 + col] = static_cast<u16_t>(VRAM_FONT_TILE_BASE + tile_index);
}

void draw_text(int col, int row, const char* str) {
    int c = col;
    while (*str) {
        if (*str == '\n') {
            ++row;
            c = col;
        } else {
            put_char(c++, row, *str);
        }
        ++str;
    }
}

static void scroll_up() {
    for (int r = 0; r < TILEMAP_ROWS - 1; ++r) {
        for (int c = 0; c < 32; ++c) {
            TILEMAP_VRAM[r * 32 + c] = TILEMAP_VRAM[(r + 1) * 32 + c];
        }
    }
    for (int c = 0; c < 32; ++c) {
        TILEMAP_VRAM[(TILEMAP_ROWS - 1) * 32 + c] = 0;
    }
}

static void new_line() {
    cursor_col = 0;
    if (cursor_row + 1 >= TILEMAP_ROWS) {
        scroll_up();
    } else {
        ++cursor_row;
    }
}

void write(char c) {
    if (c == '\n') {
        new_line();
        return;
    }
    if (c == '\r') {
        cursor_col = 0;
        return;
    }
    if (cursor_col >= TILEMAP_COLS) {
        new_line();
    }
    put_char(cursor_col++, cursor_row, c);
}

void write(const char* str) {
    while (*str) {
        write(*str++);
    }
}

void write_int(int val) {
    if (val < 0) {
        write('-');
        val = -val;
    }
    int tens = 0;
    while (val >= 10) {
        val -= 10;
        ++tens;
    }
    if (tens > 0) {
        if (tens >= 10) {
            write_int(tens);
        } else {
            write(static_cast<char>('0' + tens));
        }
    }
    write(static_cast<char>('0' + val));
}
