#include <stdint.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

// clear
#ifdef _WIN32
	#define ClearScreen() system("cls");
#else
	#define ClearScreen() system("clear");
#endif

// sleep
#ifdef _WIN32
	#define Sleep(ms) Sleep(ms);
#else
	#define Sleep(ms) usleep(ms * 1000);
#endif

// set cursor position
#ifdef _WIN32
    #define SetCursorPos(x, y) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 })
#else
	#define SetCursorPos(x, y) printf("\033[%d;%dH", x + 1, y + 1)
#endif