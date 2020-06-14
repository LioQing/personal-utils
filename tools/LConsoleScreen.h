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
#include <string>
#include <cwchar>

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
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

enum COLOUR
{
	FG_BLACK        = 0x0000,
	FG_DARK_BLUE    = 0x0001,	
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY         = 0x0007,
	FG_DARK_GREY    = 0x0008,
	FG_BLUE         = 0x0009,
	FG_GREEN		= 0x000A,
	FG_CYAN			= 0x000B,
	FG_RED          = 0x000C,
	FG_MAGENTA      = 0x000D,
	FG_YELLOW       = 0x000E,
	FG_WHITE        = 0x000F,
	BG_BLACK        = 0x0000,
	BG_DARK_BLUE    = 0x0010,
	BG_DARK_GREEN   = 0x0020,
	BG_DARK_CYAN    = 0x0030,
	BG_DARK_RED     = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW  = 0x0060,
	BG_GREY         = 0x0070,
	BG_DARK_GREY    = 0x0080,
	BG_BLUE         = 0x0090,
	BG_GREEN        = 0x00A0,
	BG_CYAN         = 0x00B0,
	BG_RED          = 0x00C0,
	BG_MAGENTA      = 0x00D0,
	BG_YELLOW       = 0x00E0,
	BG_WHITE        = 0x00F0,
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

		// font
		m_cfi.cbSize = sizeof(m_cfi);
        m_cfi.nFont = 0;
        m_cfi.dwFontSize.X = 8;
        m_cfi.dwFontSize.Y = 16;
        m_cfi.FontFamily = FF_DONTCARE;
        m_cfi.FontWeight = FW_NORMAL;
		std::wcscpy(m_cfi.FaceName, L"Consolas");
    }

    ~LConsoleScreen()
    {
        delete[] m_bufscrn;
    }

    void Init(int width, int height, int fontw = 8, int fonth = 16, std::string title = "LConsoleScreen")
    {
        //set title
        SetTitle(title);

        // set resolution
        SetResolution(width, height);

        // set buffer size
        SetConsoleScreenBufferSize(m_wHnd, m_size);

        // set font info
		SetFontSize(fontw, fonth);

		// window size
		RefreshWindowSize();

        // alloc mem for scrn
        m_bufscrn = new CHAR_INFO[m_size.X * m_size.Y];
		memset(m_bufscrn, 0, sizeof(CHAR_INFO) * m_size.X * m_size.Y);
    }

    void Display()
    {
        SMALL_RECT win = { 0, 0, (short)(m_size.X - 1), (short)(m_size.Y - 1) };
        WriteConsoleOutputA(m_wHnd, m_bufscrn, { m_size.X, m_size.Y }, { 0, 0 }, &win);
    }

	void SetTitle(std::string title) { m_title = title; SetConsoleTitleA(LPCSTR(title.c_str())); }
	std::string GetTitle() { return m_title; }

	void SetFontSize(int width, int height)
	{
		m_cfi.dwFontSize.X = width;
		m_cfi.dwFontSize.Y = height;
		SetCurrentConsoleFontEx(m_wHnd, false, &m_cfi);
	}
	COORD GetFontSize() { return m_cfi.dwFontSize; }

	void SetResolution(int width, int height) { m_size = { (short)width, (short)height }; }
	COORD GetResolution() { return m_size; }

	void RefreshWindowSize()
	{
		m_winSize = { 0, 0, (short)(m_size.X - 1), (short)(m_size.Y - 1) };
        SetConsoleWindowInfo(m_wHnd, true, &m_winSize);
	}

    /*  DRAW METHODS  */

    void Draw(int x, int y, unsigned char c = 0xdb, short col = 0x000F)
    {
        if (x >= 0 && x < m_size.X && y >= 0 && y < m_size.Y)
		{
			m_bufscrn[y * m_size.X + x].Char.AsciiChar = c;
			m_bufscrn[y * m_size.X + x].Attributes = col;
		}
    }

    void Fill(int x1, int y1, int x2, int y2, unsigned char c = 0xdb, short col = 0x000F)
    {
        Clip(x1, y1);
        Clip(x2, y2);
        for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, c, col);
    }

    void DrawString(int x, int y, std::string c, short col = 0x000F)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			m_bufscrn[y * m_size.X + x + i].Char.UnicodeChar = c[i];
			m_bufscrn[y * m_size.X + x + i].Attributes = col;
		}
	}

	void DrawStringAlpha(int x, int y, std::string c, short col = 0x000F)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			if (c[i] != ' ')
			{
				m_bufscrn[y * m_size.X + x + i].Char.UnicodeChar = c[i];
				m_bufscrn[y * m_size.X + x + i].Attributes = col;
			}
		}
	}

    void Clip(int &x, int &y)
	{
		if (x < 0) x = 0;
		if (x >= m_size.X) x = m_size.X;
		if (y < 0) y = 0;
		if (y >= m_size.Y) y = m_size.Y;
	}

	void DrawLine(int x1, int y1, int x2, int y2, unsigned char c = 0xdb, short col = 0x000F)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			Draw(x, y, c, col);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, c, col);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			Draw(x, y, c, col);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, c, col);
			}
		}
	}

	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c = 0xdb, short col = 0x000F)
	{
		DrawLine(x1, y1, x2, y2, c, col);
		DrawLine(x2, y2, x3, y3, c, col);
		DrawLine(x3, y3, x1, y1, c, col);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c = 0xdb, short col = 0x000F)
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, c, col); };
		
		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										 // Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);   										
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y>y3) return;
		}
	}

	void DrawCircle(int xc, int yc, int r, unsigned char c = 0xdb, short col = 0x000F)
	{
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) // only formulate 1/8 of circle
		{
			Draw(xc - x, yc - y, c, col);//upper left left
			Draw(xc - y, yc - x, c, col);//upper upper left
			Draw(xc + y, yc - x, c, col);//upper upper right
			Draw(xc + x, yc - y, c, col);//upper right right
			Draw(xc - x, yc + y, c, col);//lower left left
			Draw(xc - y, yc + x, c, col);//lower lower left
			Draw(xc + y, yc + x, c, col);//lower lower right
			Draw(xc + x, yc + y, c, col);//lower right right
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	}

	void FillCircle(int xc, int yc, int r, unsigned char c = 0xdb, short col = 0x000F)
	{
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, c, col);
		};

		while (y >= x)
		{
			// Modified to draw scan-lines instead of edges
			drawline(xc - x, xc + x, yc - y);
			drawline(xc - y, xc + y, yc - x);
			drawline(xc - x, xc + x, yc + y);
			drawline(xc - y, xc + y, yc + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	}

protected:

    CHAR_INFO* m_bufscrn;
    COORD m_size;
	SMALL_RECT m_winSize;
	CONSOLE_FONT_INFOEX m_cfi;
	std::string m_title;

    HANDLE m_wHnd;
    HANDLE m_rHnd;

};