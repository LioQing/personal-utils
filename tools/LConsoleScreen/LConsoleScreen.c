#include "LConsoleScreen.h"

#include <stdlib.h>
#include <string.h>

void tcs_Init(LConsoleScreen* lcs, int width, int height, int size, const char* title, short background_col)
{
	lcs->is_tcs = true;
	lcs_Init(lcs, width * 2, height, size / 2, size, title, background_col);
	lcs->md_size = (COORD){ (short)width, (short)height };
}

void lcs_Init(LConsoleScreen* lcs, int width, int height, int fontw, int fonth, const char* title, short background_col)
{
	// get handles
	lcs->m_wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	lcs->m_rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// font
	lcs->m_cfi.cbSize = sizeof(lcs->m_cfi);
	lcs->m_cfi.nFont = 0;
	lcs->m_cfi.dwFontSize.X = 8;
	lcs->m_cfi.dwFontSize.Y = 16;
	lcs->m_cfi.FontFamily = FF_DONTCARE;
	lcs->m_cfi.FontWeight = FW_NORMAL;
	wcscpy(lcs->m_cfi.FaceName, L"Consolas");

	//set title
	lcs->m_title = (char*)malloc(sizeof(char));
	lcs_SetTitle(lcs, title);

	// set resolution
	lcs_SetResolution(lcs, width, height);

	// set buffer size
	SetConsoleScreenBufferSize(lcs->m_wHnd, lcs->m_size);

	// set font info
	lcs_SetFontSize(lcs, fontw, fonth);

	// window size
	lcs_RefreshWindowSize(lcs);

	// alloc mem for scrn
	lcs->m_bufscrn = (CHAR_INFO*)calloc(lcs->m_size.X * lcs->m_size.Y, sizeof(CHAR_INFO));

	// set to background
	lcs_SetBackgroundColor(lcs, background_col);
	lcs_Clear(lcs);
}

void lcs_Destroy(LConsoleScreen* lcs)
{
	free(lcs->m_bufscrn);
}

void lcs_Display(LConsoleScreen* lcs)
{
	SMALL_RECT win = { 0, 0, (short)(lcs->m_size.X - 1), (short)(lcs->m_size.Y - 1) };
	WriteConsoleOutputA(lcs->m_wHnd, lcs->m_bufscrn, (COORD) { lcs->m_size.X, lcs->m_size.Y }, (COORD) { 0, 0 }, & win);
}

void lcs_SetTitle(LConsoleScreen* lcs, const char* title)
{
	realloc(lcs->m_title, strlen(title) * sizeof(char));
	strcpy(lcs->m_title, title);
}

void lcs_SetFontSize(LConsoleScreen* lcs, int width, int height)
{
	lcs->m_cfi.dwFontSize.X = width;
	lcs->m_cfi.dwFontSize.Y = height;
	SetCurrentConsoleFontEx(lcs->m_wHnd, false, &lcs->m_cfi);
}

void lcs_SetResolution(LConsoleScreen* lcs, int width, int height)
{
	lcs->m_size = (COORD){ (short)width, (short)height };
}

void lcs_RefreshWindowSize(LConsoleScreen* lcs)
{
	lcs->m_winSize = (SMALL_RECT){ 0, 0, (short)(lcs->m_size.X - 1), (short)(lcs->m_size.Y - 1) };
	SetConsoleWindowInfo(lcs->m_wHnd, true, &lcs->m_winSize);
}

void lcs_SetBackgroundColor(LConsoleScreen* lcs, short col)
{
	lcs->background_col = col;
}

void lcs_Draw(LConsoleScreen* lcs, int x, int y, unsigned char c, short col)
{
	if (!lcs->is_tcs && x >= 0 && x < lcs->m_size.X && y >= 0 && y < lcs->m_size.Y)
	{
		lcs->m_bufscrn[y * lcs->m_size.X + x].Char.AsciiChar = c;
		lcs->m_bufscrn[y * lcs->m_size.X + x].Attributes = col;
	}
	else if (lcs->is_tcs && x >= 0 && x < lcs->m_size.X && y >= 0 && y < lcs->m_size.Y)
	{
		lcs->m_bufscrn[y * lcs->m_size.X + x * 2].Char.AsciiChar = c;
		lcs->m_bufscrn[y * lcs->m_size.X + x * 2].Attributes = col;
		lcs->m_bufscrn[y * lcs->m_size.X + x * 2 + 1].Char.AsciiChar = c;
		lcs->m_bufscrn[y * lcs->m_size.X + x * 2 + 1].Attributes = col;
	}
}

void lcs_Fill(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, unsigned char c, short col)
{
	lcs_Clip(lcs, &x1, &y1);
	lcs_Clip(lcs, &x2, &y2);
	for (size_t x = x1; x < x2; x++)
		for (size_t y = y1; y < y2; y++)
			lcs_Draw(lcs, x, y, c, col);
}

void lcs_FullFill(LConsoleScreen* lcs, unsigned char c, short col)
{
	lcs_Fill(lcs, 0, 0, lcs->m_size.X, lcs->m_size.Y, c, col);
}

void lcs_Clear(LConsoleScreen* lcs)
{
	lcs_FullFill(lcs, ' ', lcs->background_col);
}

void lcs_DrawString(LConsoleScreen* lcs, int x, int y, const char* c, short col)
{
	for (size_t i = 0; i < strlen(c); i++)
	{
		lcs->m_bufscrn[y * lcs->m_size.X + x + i].Char.UnicodeChar = c[i];
		lcs->m_bufscrn[y * lcs->m_size.X + x + i].Attributes = col;
	}
}

void lcs_DrawStringAlpha(LConsoleScreen* lcs, int x, int y, const char* c, short col)
{
	for (size_t i = 0; i < strlen(c); i++)
	{
		if (c[i] != ' ')
		{
			lcs->m_bufscrn[y * lcs->m_size.X + x + i].Char.UnicodeChar = c[i];
			lcs->m_bufscrn[y * lcs->m_size.X + x + i].Attributes = col;
		}
	}
}

void lcs_Clip(LConsoleScreen* lcs, int* x, int* y)
{
	if (*x < 0) *x = 0;
	if (*x >= lcs->m_size.X) *x = lcs->m_size.X - 1;
	if (*y < 0) *y = 0;
	if (*y >= lcs->m_size.Y) *y = lcs->m_size.Y - 1;
}

void lcs_DrawLine(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, unsigned char c, short col)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		lcs_Draw(lcs, x, y, c, col);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			lcs_Draw(lcs, x, y, c, col);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		lcs_Draw(lcs, x, y, c, col);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			lcs_Draw(lcs, x, y, c, col);
		}
	}
}

void lcs_DrawTriangle(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c, short col)
{
	lcs_DrawLine(lcs, x1, y1, x2, y2, c, col);
	lcs_DrawLine(lcs, x2, y2, x3, y3, c, col);
	lcs_DrawLine(lcs, x3, y3, x1, y1, c, col);
}

void SWAP(int* x, int* y)
{
	int t = *x;
	*x = *y;
	*y = t;
}

void drawline(int sx, int ex, int ny, LConsoleScreen* lcs, unsigned char c, short col)
{
	for (int i = sx; i <= ex; i++)
		lcs_Draw(lcs, i, ny, c, col);
}

void lcs_FillTriangle(LConsoleScreen* lcs, int x1, int y1, int x2, int y2, int x3, int y3, unsigned char c, short col)
{
	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(&y1, &y2); SWAP(&x1, &x2); }
	if (y1 > y3) { SWAP(&y1, &y3); SWAP(&x1, &x3); }
	if (y2 > y3) { SWAP(&y2, &y3); SWAP(&x2, &x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(&dx1, &dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(&dy2, &dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
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
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y, lcs, c, col);    // Draw line from min to max points found on the y
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
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(&dy1, &dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
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

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y, lcs, c, col);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
}

void lcs_DrawCircle(LConsoleScreen* lcs, int xc, int yc, int r, unsigned char c, short col)
{
	int x = 0;
	int y = r;
	int p = 3 - 2 * r;
	if (!r) return;

	while (y >= x) // only formulate 1/8 of circle
	{
		lcs_Draw(lcs, xc - x, yc - y, c, col);//upper left left
		lcs_Draw(lcs, xc - y, yc - x, c, col);//upper upper left
		lcs_Draw(lcs, xc + y, yc - x, c, col);//upper upper right
		lcs_Draw(lcs, xc + x, yc - y, c, col);//upper right right
		lcs_Draw(lcs, xc - x, yc + y, c, col);//lower left left
		lcs_Draw(lcs, xc - y, yc + x, c, col);//lower lower left
		lcs_Draw(lcs, xc + y, yc + x, c, col);//lower lower right
		lcs_Draw(lcs, xc + x, yc + y, c, col);//lower right right
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void lcs_FillCircle(LConsoleScreen* lcs, int xc, int yc, int r, unsigned char c, short col)
{
	// Taken from wikipedia
	int x = 0;
	int y = r;
	int p = 3 - 2 * r;
	if (!r) return;

	while (y >= x)
	{
		// Modified to draw scan-lines instead of edges
		drawline(xc - x, xc + x, yc - y, lcs, c, col);
		drawline(xc - y, xc + y, yc - x, lcs, c, col);
		drawline(xc - x, xc + x, yc + y, lcs, c, col);
		drawline(xc - y, xc + y, yc + x, lcs, c, col);
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}