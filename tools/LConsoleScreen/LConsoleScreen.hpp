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

namespace lio
{
	enum COLOUR
	{
		FG_BLACK = 0x0000,
		FG_DARK_BLUE = 0x0001,
		FG_DARK_GREEN = 0x0002,
		FG_DARK_CYAN = 0x0003,
		FG_DARK_RED = 0x0004,
		FG_DARK_MAGENTA = 0x0005,
		FG_DARK_YELLOW = 0x0006,
		FG_GREY = 0x0007,
		FG_DARK_GREY = 0x0008,
		FG_BLUE = 0x0009,
		FG_GREEN = 0x000A,
		FG_CYAN = 0x000B,
		FG_RED = 0x000C,
		FG_MAGENTA = 0x000D,
		FG_YELLOW = 0x000E,
		FG_WHITE = 0x000F,
		BG_BLACK = 0x0000,
		BG_DARK_BLUE = 0x0010,
		BG_DARK_GREEN = 0x0020,
		BG_DARK_CYAN = 0x0030,
		BG_DARK_RED = 0x0040,
		BG_DARK_MAGENTA = 0x0050,
		BG_DARK_YELLOW = 0x0060,
		BG_GREY = 0x0070,
		BG_DARK_GREY = 0x0080,
		BG_BLUE = 0x0090,
		BG_GREEN = 0x00A0,
		BG_CYAN = 0x00B0,
		BG_RED = 0x00C0,
		BG_MAGENTA = 0x00D0,
		BG_YELLOW = 0x00E0,
		BG_WHITE = 0x00F0,
	};

	enum PIXEL_TYPE
	{
		PIXEL_SOLID = 0xdb,
		PIXEL_THREEQUARTERS = 0xb2,
		PIXEL_HALF = 0xb1,
		PIXEL_QUARTER = 0xb0,
	};

	// This is the screen class
	class LConsoleScreen
	{
	public:

		LConsoleScreen();
		~LConsoleScreen();

		void Init(int width, int height, int fontw = 8, int fonth = 16, const std::string& title = "LConsoleScreen", short background_col = 0x0000);

		void Display();

		void SetTitle(std::string title) { m_title = title; SetConsoleTitleA(LPCSTR(title.c_str())); }
		std::string GetTitle() { return m_title; }

		void SetFontSize(int width, int height);
		COORD GetFontSize();

		void SetResolution(int width, int height);
		COORD GetResolution();

		void RefreshWindowSize();

		short GetBackgroundColor();
		void SetBackgroundColor(short col = 0x0000);

		/*  DRAW METHODS  */

		virtual void Draw(int x, int y, unsigned char c = 0xdb, short col = 0x000F);

		void Fill(int x1, int y1, int x2, int y2, unsigned char c = 0xdb, short col = 0x000F);

		void FullFill(unsigned char c = 0xdb, short col = 0x000F);

		void Clear();

		void DrawString(int x, int y, const std::string& c, short col = 0x000F);

		void DrawStringAlpha(int x, int y, const std::string& c, short col = 0x000F);

		void Clip(int& x, int& y);

		void DrawLine(int x1, int y1, int x2, int y2, unsigned char c = 0xdb, short col = 0x000F);

		void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c = 0xdb, short col = 0x000F);

		// https://www.avrfreaks.net/sites/default/files/triangles.c
		void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c = 0xdb, short col = 0x000F);

		void DrawCircle(int xc, int yc, int r, unsigned char c = 0xdb, short col = 0x000F);

		void FillCircle(int xc, int yc, int r, unsigned char c = 0xdb, short col = 0x000F);

	protected:

		CHAR_INFO* m_bufscrn;
		COORD m_size;
		SMALL_RECT m_winSize;
		CONSOLE_FONT_INFOEX m_cfi;
		std::string m_title;

		HANDLE m_wHnd;
		HANDLE m_rHnd;

		short background_col = 0x0000;
	};

	// This class place 2 block as 1 pixel
	// so that text will not be scaled while retaining the square shape of pixels
	class TConsoleScreen : public LConsoleScreen
	{
	public:

		TConsoleScreen();
		~TConsoleScreen();

		void Init(int width, int height, int size = 16, const std::string& title = "TConsoleScreen");

		/* DRAW METHODS */

		void Draw(int x, int y, unsigned char c = 0xdb, short col = 0x000F) override;

	private:

		COORD md_size;
	};
}