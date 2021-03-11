#include "ANSIEscaper.hpp"

#include <iostream>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
#endif

namespace lio
{
    void ANSIEscaper::Init()
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
        {
            std::cerr << "failed to get STD_OUTPUT_HANDLE" << std::endl;
            return;
        }
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE)
        {
            std::cerr << "failed to get STD_INPUT_HANDLE" << std::endl;
            return;
        }

        DWORD dwOriginalOutMode = 0;
        DWORD dwOriginalInMode = 0;
        if (!GetConsoleMode(hOut, &dwOriginalOutMode))
        {
            std::cerr << "failed to get console output mode" << std::endl;
            return;
        }
        if (!GetConsoleMode(hIn, &dwOriginalInMode))
        {
            std::cerr << "failed to get console input mode" << std::endl;
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
                std::cerr << "failed to set VT output mode" << std::endl;
                exit(-1);
            }
        }

        DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
        if (!SetConsoleMode(hIn, dwInMode))
        {
                std::cerr << "failed to set VT input mode" << std::endl;
                exit(-1);
        }

#endif
    }

    void ANSIEscaper::SetCursorPos(size_t x, size_t y)
    {
        std::cout << ESC << "[" << y << ";" << x << "H";
    }

    void ANSIEscaper::MoveCursorX(int32_t offset)
    {
        if (offset < 0)
            std::cout << ESC << "[" << -offset << "D";
        else
            std::cout << ESC << "[" << offset << "C";
    }

    void ANSIEscaper::MoveCursorY(int32_t offset)
    {
        if (offset < 0)
            std::cout << ESC << "[" << -offset << "A";
        else
            std::cout << ESC << "[" << offset << "B";
    }

    void ANSIEscaper::MoveCursorPos(int32_t offset_x, int32_t offset_y)
    {
        MoveCursorX(offset_x);
        MoveCursorY(offset_y);
    }

    void ANSIEscaper::SetTextColor4bit(Color4bit col)
    {
        std::cout << ESC << "[" << std::to_string(col) << "m";
    }

    void ANSIEscaper::SetTextColor8bit(uint8_t col, ColorType col_type)
    {
        std::cout << ESC << "[" << std::to_string(38 + col_type) << ";5;" << std::to_string(col) << "m";
    }

    void ANSIEscaper::SetTextColor24bit(uint8_t r, uint8_t g, uint8_t b, ColorType col_type)
    {
        std::cout << ESC << "[" << std::to_string(38 + col_type) << ";2;" << 
            std::to_string(r) << ";" << std::to_string(g) << ";" << std::to_string(b) << "m";
    }

    void ANSIEscaper::SetTextColor24bit(uint32_t rgb, ColorType col_type)
    {
        std::cout << ESC << "[" << std::to_string(38 + col_type) << ";2;" << 
            std::to_string(rgb / (uint32_t)0x00'01'00'00 % 0x1'00) << ";" << 
            std::to_string(rgb / (uint32_t)0x00'00'01'00 % 0x1'00) << ";" << 
            std::to_string(rgb / (uint32_t)0x00'00'00'01 % 0x1'00) << "m";
    }

    void ANSIEscaper::SetTextAttributes(TextAttribute attr)
    {
        std::cout << ESC << "[0m";

        if (attr & BOLD)                std::cout << ESC << "[1m";
        else if (attr & DIM)            std::cout << ESC << "[2m";
        else if (attr & ITALIC)         std::cout << ESC << "[3m";
        else if (attr & UNDERLINE)      std::cout << ESC << "[4m";
        else if (attr & BLINK)          std::cout << ESC << "[5m";
        else if (attr & INVERSED)       std::cout << ESC << "[7m";
        else if (attr & INVISIBLE)      std::cout << ESC << "[8m";
        else if (attr & STRIKETHROUGH)  std::cout << ESC << "[9m";
    }

    void ANSIEscaper::ClearScreen()
    {
        std::cout << ESC << "[2J";
    }
}