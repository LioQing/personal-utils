#include "ANSIEscaper.h"

#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
#endif

void ANSIEscaperInit()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "failed to get STD_OUTPUT_HANDLE\n");
        return;
    }
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "failed to get STD_INPUT_HANDLE\n");
        return;
    }

    DWORD dwOriginalOutMode = 0;
    DWORD dwOriginalInMode = 0;
    if (!GetConsoleMode(hOut, &dwOriginalOutMode))
    {
        fprintf(stderr, "failed to get console output mode\n");
        return;
    }
    if (!GetConsoleMode(hIn, &dwOriginalInMode))
    {
        fprintf(stderr, "failed to get console input mode\n");
        return;
    }

    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
    if (!SetConsoleMode(hOut, dwOutMode))
    {
        dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
        if (!SetConsoleMode(hOut, dwOutMode))
        {
            fprintf(stderr, "failed to set VT output mode\n");
            exit(-1);
        }
    }

    DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(hIn, dwInMode))
    {
        fprintf(stderr, "failed to set VT input mode\n");
        exit(-1);
    }

#endif
}

void ANSIEscaperSetCursorPos(size_t x, size_t y)
{
    printf("\x1b[%zu;%zuH", y, x);
}

void ANSIEscaperMoveCursorX(int32_t offset)
{
    if (offset < 0)
        printf("\x1b[%dD", -offset);
    else
        printf("\x1b[%dC", offset);
}

void ANSIEscaperMoveCursorY(int32_t offset)
{
    if (offset < 0)
        printf("\x1b[%dA", -offset);
    else
        printf("\x1b[%dB", offset);
}

void ANSIEscaperMoveCursorPos(int32_t offset_x, int32_t offset_y)
{
    ANSIEscaperMoveCursorX(offset_x);
    ANSIEscaperMoveCursorY(offset_y);
}

void ANSIEscaperSetTextColor4bit(int col)
{
    printf("\x1b[%dm", col);
}

void ANSIEscaperSetTextColor8bit(uint8_t col, int col_type)
{
    printf("\x1b[%d;5;%um", col_type + 38, col);
}

void ANSIEscaperSetTextColor24bit(uint8_t r, uint8_t g, uint8_t b, int col_type)
{
    printf("\x1b[%d;2;%u;%u;%um", col_type + 38, r, g, b);
}

void ANSIEscaperSetTextColor24bitRGB(uint32_t rgb, int col_type)
{
    printf("\x1b[%d;2;%u;%u;%um", col_type + 38, 
        rgb / (uint32_t)0x00010000 % 0x100, 
        rgb / (uint32_t)0x00000100 % 0x100, 
        rgb / (uint32_t)0x00000001 % 0x100);
}

void ANSIEscaperSetTextAttributes(int attr)
{
    printf("\x1b[0m");

    if (attr & BOLD)                printf("\x1b[1m");
    else if (attr & DIM)            printf("\x1b[2m");
    else if (attr & ITALIC)         printf("\x1b[3m");
    else if (attr & UNDERLINE)      printf("\x1b[4m");
    else if (attr & BLINK)          printf("\x1b[5m");
    else if (attr & INVERSED)       printf("\x1b[7m");
    else if (attr & INVISIBLE)      printf("\x1b[8m");
    else if (attr & STRIKETHROUGH)  printf("\x1b[9m");
}

void ANSIEscaperClearScreen()
{
    printf("\x1b[2J");
}