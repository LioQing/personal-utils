#include <stdint.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

// clear
#ifdef _WIN32
	#define clear_screen() system("cls")
#else
	#define clear_screen() system("clear")
#endif

// sleep
#ifdef _WIN32
	#define sleep(ms) Sleep(ms)
#else
	#define sleep(ms) usleep(ms * 1000)
#endif

// set cursor position
#ifdef _WIN32
    #define set_cursor_pos(x, y) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 })
#else
    #define set_cursor_pos(x, y) printf("\033[%d;%dH", x + 1, y + 1)
#endif
