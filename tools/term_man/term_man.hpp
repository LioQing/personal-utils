#pragma once

#include <cstdint>
#include <string>
#include <signal.h>

namespace tman
{
    struct SystemEvent
    {
        enum
        {
            Exit, Resize
        };

        sig_atomic_t type;
    };

    struct InputEvent
    {
        enum : uint8_t
        {
            // escape codes
            F0,
            F1, F2, F3, F4,
            F5, F6, F7, F8,
            Up, Down, Right, Left,
            Home, Insert, Delete, End, PgUp, PgDown,

            // normal codes (named)
            Enter = 13,
            Escape = 27,
            Tab = 9,
        };

        uint8_t code;
        bool is_esc;
        bool is_alt;
    };

    struct Event
    {
        enum Type
        {
            System, Input,
        };

        Type type;

        union
        {
            SystemEvent system;
            InputEvent input;
        };
    };

    bool Init();
    bool End();

    void SetCursorPos(uint16_t x, uint16_t y);
    
    void SetColor4bit(uint8_t fg_col, uint8_t bg_col);
    void SetColor8bit(uint8_t fg_col, uint8_t bg_col);
    void SetColor24bit(uint8_t fg_r, uint8_t fg_g, uint8_t fg_b, uint8_t bg_r, uint8_t bg_g, uint8_t bg_b);
    void SetColor24bit(uint32_t fg_rgb, uint32_t bg_rgb);
    void ResetColor();
    
    bool UpdateSize();
    int16_t GetSizeX();
    int16_t GetSizeY();
    void GetSize(int16_t& x, int16_t& y);

    void HideCursor();
    void ShowCursor();

    bool GetInputBuf(std::string& buf);
    bool PollEvent(Event& event);
}