#include <stdint.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

// clear
void ClearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

// sleep
void Sleep(uint32_t ms)
{
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}