#include <iostream>
#include <thread>
#include <cmath>
#include <sys/ioctl.h>
#include <termios.h>

#include "tcon.hpp"

int main()
{
    // init
    tcon::Handle handle;
    if (!handle.Init())
    {
        handle.End();
        std::cerr << "Failed to initialize tcon::Handle" << std::endl;
        return 1;
    }
    tcon::HideCursor();
    tcon::ClearScreen();

    // variables
    bool is_running = true;

    char hello_world[] = "Hello World!";
    int32_t col[] = { 0xff0000, 0xff0000, 0xff7f00, 0xff7f00, 0xffff00, 0xffff00, 0x00ff00, 0x00ff00, 0x0000ff, 0x0000ff, 0x9400d3, 0x9400d3 };
    float len = 0.f;

    char circle[] = "circle";
    float theta = 0.f;
    int rot_dir = 1;
    int radius = 10;

    // main loop
    while (is_running)
    {
        // event polling
        tcon::Event event;
        while (handle.PollEvent(event))
        {
            if (event.type == tcon::Event::Exit)        // exit event
            {
                is_running = false;
            }
            else if (event.type == tcon::Event::Input)  // input event
            {
                if (!event.input.is_esc && !event.input.is_alt && event.input.code == ' ') // space pressed
                {
                    rot_dir = -rot_dir;
                }
            }
        }

        // update
        len = fmod(len + .33f, 16);
        theta = fmod(theta + rot_dir * .05f, 2 * 3.1415926);

        // draw
        tcon::SetStyle(tcon::Style::All, false);
        tcon::ResetColor(tcon::Target::Foreground);
        tcon::ResetColor(tcon::Target::Background);
        tcon::ClearScreen();

        // hello world
        tcon::SetCursorPos(handle.GetWidth() / 2 - 6, handle.GetHeight() / 2);
        tcon::SetStyle(tcon::Style::Italic | tcon::Style::Underline, true);
        for (int i = 0; i < 12; ++i)
        {
            if (i < len)
            {
                tcon::SetColor24bit(col[i], tcon::Target::Foreground);
                printf("%c", hello_world[i]);
            }
            else
            {
                printf(" ");
            }
        }

        // circle
        tcon::SetCursorPos(handle.GetWidth() / 2 - 2 + (int)(2 * radius * cos(theta)), handle.GetHeight() / 2 + (int)(radius * sin(theta)));
        tcon::SetStyle(tcon::Style::All, false);
        tcon::SetColor4bit(tcon::Color::BrightBlue, tcon::Target::Foreground);
        tcon::ResetColor(tcon::Target::Background);
        printf("%s", circle);

        fflush(stdout);

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // clean up
    tcon::ClearScreen();
    tcon::ShowCursor();

    return 0;
}