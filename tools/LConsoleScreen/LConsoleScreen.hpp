#pragma once

#include <windows.h>
#include <string>

namespace lio
{
	// This is the screen class
	class LConsoleScreen
	{
	public:

		enum COLOR : short
		{
			FG_BLACK = 0x0000,
			FG_DARK_BLUE = 0x0001,
			FG_DARK_GREEN = 0x0002,
			FG_DARK_CYAN = 0x0003,
			FG_DARK_RED = 0x0004,
			FG_DARK_MAGENTA = 0x0005,
			FG_DARK_YELLOW = 0x0006,
			FG_GRAY = 0x0007,
			FG_DARK_GRAY = 0x0008,
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
			BG_GRAY = 0x0070,
			BG_DARK_GRAY = 0x0080,
			BG_BLUE = 0x0090,
			BG_GREEN = 0x00A0,
			BG_CYAN = 0x00B0,
			BG_RED = 0x00C0,
			BG_MAGENTA = 0x00D0,
			BG_YELLOW = 0x00E0,
			BG_WHITE = 0x00F0,
		};

		enum PIXEL_TYPE : unsigned char
		{
			PIXEL_SOLID = 0xdb,
			PIXEL_THREE_QUARTERS = 0xb2,
			PIXEL_HALF = 0xb1,
			PIXEL_QUARTER = 0xb0,
		};
		
		LConsoleScreen();
		~LConsoleScreen();

		void Init(int width, int height, int fontw = 8, int fonth = 16, const std::string& title = "LConsoleScreen", short background_col = 0x0000);

		void Display();

		void SetTitle(const std::string& title);
		const std::string& GetTitle() const;

		void SetFontSize(int width, int height);
		COORD GetFontSize() const;

		void SetResolution(int width, int height);
		COORD GetResolution() const;

		void RefreshWindowSize();

		short GetBackgroundColor() const;
		void SetBackgroundColor(short col = 0x0000);

		/*  DRAW METHODS  */

		virtual void Draw(int x, int y, unsigned char c = 0xdb, short col = 0x000f);

		void Fill(int x1, int y1, int x2, int y2, unsigned char c = 0xdb, short col = 0x000f);

		void FullFill(unsigned char c = 0xdb, short col = 0x000f);

		void Clear();

		void DrawString(int x, int y, const std::string& c, short col = 0x000f);

		void DrawStringAlpha(int x, int y, const std::string& c, short col = 0x000f);

		void Clip(int& x, int& y) const;

		void DrawLine(int x1, int y1, int x2, int y2, unsigned char c = 0xdb, short col = 0x000f);

		void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c = 0xdb, short col = 0x000f);

		// https://www.avrfreaks.net/sites/default/files/triangles.c
		void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c = 0xdb, short col = 0x000f);

		void DrawCircle(int xc, int yc, int r, unsigned char c = 0xdb, short col = 0x000f);

		void FillCircle(int xc, int yc, int r, unsigned char c = 0xdb, short col = 0x000f);

	protected:

		CHAR_INFO* m_bufscrn;
		COORD m_size;
		SMALL_RECT m_winSize;
		CONSOLE_FONT_INFOEX m_cfi;
		std::string m_title;

		HANDLE m_wHnd;
		HANDLE m_rHnd;

		short m_background_col = 0x0000;
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

		void Draw(int x, int y, unsigned char c = 0xdb, short col = 0x000f) override;

	private:

		COORD md_size;
	};
}