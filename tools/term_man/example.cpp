#include <thread>
#include <cmath>
#include <sys/ioctl.h>
#include <termios.h>

#include "term_man.hpp"

int main()
{
    // init
    tman::Init();
    tman::HideCursor();
    tman::ClearScreen();

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
        tman::Event event;
        while (tman::PollEvent(event))
        {
            if (event.type == tman::Event::Exit)        // exit event
            {
                is_running = false;
            }
            else if (event.type == tman::Event::Input)  // input event
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
        tman::ClearScreen();

        // hello world
        tman::SetCursorPos(tman::GetSizeX() / 2 - 6, tman::GetSizeY() / 2);
        for (int i = 0; i < 12; ++i)
        {
            if (i < len)
            {
                tman::SetColor24bit(col[i], tman::Target::Foreground);
                printf("%c", hello_world[i]);
            }
            else
            {
                printf(" ");
            }
        }

        // circle
        tman::SetCursorPos(tman::GetSizeX() / 2 - 2 + (int)(2 * radius * cos(theta)), tman::GetSizeY() / 2 + (int)(radius * sin(theta)));
        tman::ResetColor();
        printf("%s", circle);

        fflush(stdout);

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // clean up
    tman::ClearScreen();
    tman::ShowCursor();
    tman::End();

    return 0;
}