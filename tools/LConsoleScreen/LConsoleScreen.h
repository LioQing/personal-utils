/*
Majority of the code is from the internet.
This header library is specifically for windows only.
All the characters displayed in cmd are ascii characters.
(for unicode characters, you can go take a look at olcConsoleGameEngine(2nd source in the list below))
Credit for sources:
http://www.cplusplus.com/forum/beginner/134485/
https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
https://www.avrfreaks.net/sites/default/files/triangles.c
*/

#pragma once

#include <Windows.h>
#include <stdbool.h>

enum lcs_COLOUR
{
	lcs_FG_BLACK = 0x0000,
	lcs_FG_DARK_BLUE = 0x0001,
	lcs_FG_DARK_GREEN = 0x0002,
	lcs_FG_DARK_CYAN = 0x0003,
	lcs_FG_DARK_RED = 0x0004,
	lcs_FG_DARK_MAGENTA = 0x0005,
	lcs_FG_DARK_YELLOW = 0x0006,
	lcs_FG_GREY = 0x0007,
	lcs_FG_DARK_GREY = 0x0008,
	lcs_FG_BLUE = 0x0009,
	lcs_FG_GREEN = 0x000A,
	lcs_FG_CYAN = 0x000B,
	lcs_FG_RED = 0x000C,
	lcs_FG_MAGENTA = 0x000D,
	lcs_FG_YELLOW = 0x000E,
	lcs_FG_WHITE = 0x000F,
	lcs_BG_BLACK = 0x0000,
	lcs_BG_DARK_BLUE = 0x0010,
	lcs_BG_DARK_GREEN = 0x0020,
	lcs_BG_DARK_CYAN = 0x0030,
	lcs_BG_DARK_RED = 0x0040,
	lcs_BG_DARK_MAGENTA = 0x0050,
	lcs_BG_DARK_YELLOW = 0x0060,
	lcs_BG_GREY = 0x0070,
	lcs_BG_DARK_GREY = 0x0080,
	lcs_BG_BLUE = 0x0090,
	lcs_BG_GREEN = 0x00A0,
	lcs_BG_CYAN = 0x00B0,
	lcs_BG_RED = 0x00C0,
	lcs_BG_MAGENTA = 0x00D0,
	lcs_BG_YELLOW = 0x00E0,
	lcs_BG_WHITE = 0x00F0,
};

enum lcs_PIXEL_TYPE
{
	lcs_PIXEL_SOLID = 0xdb,
	lcs_PIXEL_THREEQUARTERS = 0xb2,
	lcs_PIXEL_HALF = 0xb1,
	lcs_PIXEL_QUARTER = 0xb0,
};

typedef struct LConsoleScreen
{
	CHAR_INFO* m_bufscrn;
	COORD m_size;
	SMALL_RECT m_winSize;
	CONSOLE_FONT_INFOEX m_cfi;
	char* m_title;

	HANDLE m_wHnd;
	HANDLE m_rHnd;

	short background_col;

	bool is_tcs;
	COORD md_size;
} LConsoleScreen;

void tcs_Init(LConsoleScreen* lcs, int width, int height, int size, const char* title, short background_col);
void lcs_Init(LConsoleScreen* lcs, int width, int height, int fontw, int fonth, const char* title, short background_col);
void lcs_Destroy(LConsoleScreen* lcs);

void lcs_Display(LConsoleScreen* lcs);

void lcs_SetTitle(LConsoleScreen* lcs, const char* title);

void lcs_SetFontSize(LConsoleScreen* lcs, int width, int height);

void lcs_SetResolution(LConsoleScreen* lcs, int width, int height);

void lcs_RefreshWindowSize(LConsoleScreen* lcs);

void lcs_SetBackgroundColor(LConsoleScreen* lcs, short col);

/*  DRAW METHODS  */

void lcs_Draw(LConsoleScreen* lcs, int x, int y, unsigned char c, short col);

void lcs_Fill(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, unsigned char c, short col);

void lcs_FullFill(LConsoleScreen* lcs, unsigned char c, short col);

void lcs_Clear(LConsoleScreen* lcs);

void lcs_DrawString(LConsoleScreen* lcs, int x, int y, const char* c, short col);

void lcs_DrawStringAlpha(LConsoleScreen* lcs, int x, int y, const char* c, short col);

void lcs_Clip(LConsoleScreen* lcs, int* x, int* y);

void lcs_DrawLine(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, unsigned char c, short col);

void lcs_DrawTriangle(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c, short col);

// https://www.avrfreaks.net/sites/default/files/triangles.c
void lcs_FillTriangle(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c, short col);

void lcs_DrawCircle(LConsoleScreen* lcs, int xc, int yc, int r, unsigned char c, short col);

void lcs_FillCircle(LConsoleScreen* lcs, int xc, int yc, int r, unsigned char c, short col);
