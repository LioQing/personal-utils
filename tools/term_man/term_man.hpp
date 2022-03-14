#pragma once

#include <cstdint>
#include <string>
#include <signal.h>

namespace tman
{
    /**
     * @brief A structure containing input event information
     * 
     */
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

    /**
     * @brief A union of all the event structures
     * 
     */
    struct Event
    {
        enum Type
        {
            Exit, Resize, Input,
        };

        Type type;

        union
        {
            InputEvent input;
        };
    };

    enum class Target
    {
        Foreground = 38, Background = 48,
    };

    /**
     * @brief Initialize term_man.
     * 
     * @return A boolean indicating success or not
     */
    bool Init();

    /**
     * @brief Restore default terminal and dettach signal handler.
     * 
     * @return A boolean indicating success or not
     */
    bool End();

    /**
     * @brief Set the cursor position.
     * 
     * @param x The x coordinate
     * @param y The y coordinate
     */
    void SetCursorPos(uint16_t x, uint16_t y);
    
    /**
     * @brief Set the color in 4 bit format (16 colors).
     * 
     * @param fg_col Foreground color
     * @param bg_col Background color
     */
    void SetColor4bit(uint8_t fg_col, uint8_t bg_col);

    /**
     * @brief Set the color in 8 bit format (256 colors)
     * 
     * @param col The color to be set
     * @param target The target (foreground or background)
     */
    void SetColor8bit(uint8_t col, Target target);

    /**
     * @brief Set the color in 24 bit format (rgb color).
     * 
     * @param r Red
     * @param g Breen
     * @param b Blue
     * @param target The target (foreground or background)
     */
    void SetColor24bit(uint8_t r, uint8_t g, uint8_t b, Target target);

    /**
     * @brief Set the color in 24 bit format (rgb color).
     * 
     * @param rgb The color to be set
     * @param target The target (foreground or background)
     */
    void SetColor24bit(uint32_t rgb, Target target);

    /**
     * @brief Set the color to default.
     * 
     */
    void ResetColor();

    /**
     * @brief Clear the screen, cursor position will be (0, 0) after this.
     * 
     */
    void ClearScreen();
    
    /**
     * @brief Update the terminal size.
     * 
     * @return A boolean indicating success or not
     */
    bool UpdateSize();

    /**
     * @brief Get the width of terminal.
     * 
     * @return int16_t The width
     */
    int16_t GetSizeX();

    /**
     * @brief Get the height of terminal.
     * 
     * @return int16_t The height
     */
    int16_t GetSizeY();

    /**
     * @brief Get the size of terminal.
     * 
     * @param x The width value to be returned
     * @param y The height value to be returned
     */
    void GetSize(int16_t& x, int16_t& y);

    /**
     * @brief Hide the cursor.
     * 
     */
    void HideCursor();

    /**
     * @brief Show the cursor.
     * 
     */
    void ShowCursor();

    /**
     * @brief Poll events.
     * 
     * @param event The Event struct to be returned
     * @return A boolean indicating whether an event is polled
     */
    bool PollEvent(Event& event);
}