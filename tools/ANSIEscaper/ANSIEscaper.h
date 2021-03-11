#pragma once

#include <stdlib.h>
#include <stdint.h>

enum Code
{
    BEL = 0x07,
    BS = 0x08,
    HT = 0x09,
    LF = 0x0A,
    VT = 0x0B,
    FF = 0x0C,
    CR = 0x0D,
    ESC = 0x1B,
    DEL = 0x7F,
};

enum Color4bit
{
    FG_BLACK = 30,
    FG_RED,
    FG_GREEN,
    FG_YELLOW,
    FG_BLUE,
    FG_MAGENTA,
    FG_CYAN,
    FG_WHITE,
    BG_BLACK = 40,
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE,
    FG_BRIGHT_BLACK = 90,
    FG_BRIGHT_RED,
    FG_BRIGHT_GREEN,
    FG_BRIGHT_YELLOW,
    FG_BRIGHT_BLUE,
    FG_BRIGHT_MAGENTA,
    FG_BRIGHT_CYAN,
    FG_BRIGHT_WHITE,
    BG_BRIGHT_BLACK = 100,
    BG_BRIGHT_RED,
    BG_BRIGHT_GREEN,
    BG_BRIGHT_YELLOW,
    BG_BRIGHT_BLUE,
    BG_BRIGHT_MAGENTA,
    BG_BRIGHT_CYAN,
    BG_BRIGHT_WHITE,
};

enum Color8bit
{
    BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE,
    DARK_GRAY = 237,
    LIGHT_GRAY = 250,
};

enum ColorType
{
    FOREGROUND = 0,
    BACKGROUND = 10,
};

enum TextAttribute
{
    RESET			= 0,
    BOLD			= 0b00000001,
    DIM				= 0b00000010,
    ITALIC			= 0b00000100,
    UNDERLINE		= 0b00001000,
    BLINK			= 0b00010000,
    INVERSED		= 0b00100000,
    INVISIBLE		= 0b01000000,
    STRIKETHROUGH	= 0b10000000,
};

void ANSIEscaperInit();

void ANSIEscaperSetCursorPos(size_t x, size_t y);
void ANSIEscaperMoveCursorX(int32_t offset);
void ANSIEscaperMoveCursorY(int32_t offset);
void ANSIEscaperMoveCursorPos(int32_t offset_x, int32_t offset_y);

void ANSIEscaperSetTextColor4bit(int col);
void ANSIEscaperSetTextColor8bit(uint8_t col, int col_type);
void ANSIEscaperSetTextColor24bit(uint8_t r, uint8_t g, uint8_t b, int col_type);
void ANSIEscaperSetTextColor24bitRGB(uint32_t rgb, int col_type);

void ANSIEscaperSetTextAttributes(int TextAttribute);

void ANSIEscaperClearScreen();