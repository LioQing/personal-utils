#pragma once

#include <Windows.h>

typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX
lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

enum COLOUR
{
	FG_BLACK		= 0x0000,
	FG_DARK_BLUE    = 0x0001,	
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY			= 0x0007,
	FG_DARK_GREY    = 0x0008,
	FG_BLUE			= 0x0009,
	FG_GREEN		= 0x000A,
	FG_CYAN			= 0x000B,
	FG_RED			= 0x000C,
	FG_MAGENTA		= 0x000D,
	FG_YELLOW		= 0x000E,
	FG_WHITE		= 0x000F,
	BG_BLACK		= 0x0000,
	BG_DARK_BLUE	= 0x0010,
	BG_DARK_GREEN	= 0x0020,
	BG_DARK_CYAN	= 0x0030,
	BG_DARK_RED		= 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW	= 0x0060,
	BG_GREY			= 0x0070,
	BG_DARK_GREY	= 0x0080,
	BG_BLUE			= 0x0090,
	BG_GREEN		= 0x00A0,
	BG_CYAN			= 0x00B0,
	BG_RED			= 0x00C0,
	BG_MAGENTA		= 0x00D0,
	BG_YELLOW		= 0x00E0,
	BG_WHITE		= 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0xdb,
	PIXEL_THREEQUARTERS = 0xb2,
	PIXEL_HALF = 0xb1,
	PIXEL_QUARTER = 0xb0,
};

class LConsoleScreen
{
public:

    LConsoleScreen()
    {
        // get handles
        m_wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
        m_rHnd = GetStdHandle(STD_INPUT_HANDLE);
    }

    void Init(int width, int height, int fontw = 8, int fonth = 16)
    {
        // set size
        m_size = { (short)(width - 1), (short)(height - 1) };

        //set title
        SetConsoleTitle(TEXT("TEST"));

        // set windows size
        SMALL_RECT win_size = { 0, 0, m_size.X, m_size.Y };
        SetConsoleWindowInfo(m_wHnd, TRUE, &win_size);

        // set buffer size
        SetConsoleScreenBufferSize(m_wHnd, m_size);

        // set font info
        CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontw;
		cfi.dwFontSize.Y = fonth;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

        wcsncpy(cfi.FaceName, L"Consolas", LF_FACESIZE);
        SetCurrentConsoleFontEx(m_wHnd, false, &cfi);

        // alloc mem for scrn
        m_bufscrn = new CHAR_INFO[m_size.X * m_size.Y];
		memset(m_bufscrn, 0, sizeof(CHAR_INFO) * m_size.X * m_size.Y);
    }

    void Display()
    {
        SMALL_RECT win = { 0, 0, (short)(m_size.X - 1), (short)(m_size.Y - 1) };
        WriteConsoleOutputA(m_wHnd, m_bufscrn, { m_size.X, m_size.Y }, { 0, 0 }, &win);
    }

    void Draw(int x, int y, unsigned char c = 0xdb, short col = 0x000F)
    {
        if (x >= 0 && x < m_size.X && y >= 0 && y < m_size.Y)
		{
			m_bufscrn[y * m_size.X + x].Char.AsciiChar = c;
			m_bufscrn[y * m_size.X + x].Attributes = col;
		}
    }

protected:

    CHAR_INFO* m_bufscrn;
    COORD m_size;

    HANDLE m_wHnd;
    HANDLE m_rHnd;

};