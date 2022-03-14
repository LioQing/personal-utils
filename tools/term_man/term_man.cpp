#include "term_man.hpp"

#include <deque>
#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

namespace tman
{
    namespace
    {
        constexpr char ESC = 0x1b;

        enum Signal : sig_atomic_t
        {
            None    = 0,
            Exit    = 1 << 0,
            Resize  = 1 << 1,
        };

        volatile sig_atomic_t signal_status = Signal::None;
        void SAHandler(int32_t signal);

        int32_t MapEscSeqToInputCode(const char* esc_seq, int32_t size, uint8_t& code, bool& is_alt);

        uint16_t size_x;
        uint16_t size_y;

        termios init_term;

        std::deque<Event> event_queue;
    }

    bool Init()
    {
        tcgetattr(STDIN_FILENO, &init_term);

        event_queue = std::deque<Event>();

        termios term = init_term;
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);

        struct sigaction sa;
        sa.sa_handler = &SAHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGWINCH, &sa, NULL) != 0)
            return false;

        if (sigaction(SIGINT, &sa, NULL) != 0)
            return false;

        UpdateSize();

        return true;
    }

    bool End()
    {
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGWINCH, &sa, NULL) != 0)
            return false;

        if (sigaction(SIGINT, &sa, NULL) != 0)
            return false;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &init_term) != 0)
            return false;

        return true;
    }

    void SetCursorPos(uint16_t x, uint16_t y)
    {
        std::printf("%c[%d;%df", ESC, y, x);
    }

    void SetColor4bit(uint8_t fg_col, uint8_t bg_col)
    {
        std::printf("%c[%d;%dm", ESC, fg_col, bg_col);
    }

    void SetColor8bit(uint8_t fg_col, uint8_t bg_col)
    {
        std::printf("%c[38;5;%d;48;5;%dm", ESC, fg_col, bg_col);
    }

    void SetColor24bit(uint8_t fg_r, uint8_t fg_g, uint8_t fg_b, uint8_t bg_r, uint8_t bg_g, uint8_t bg_b)
    {
        std::printf("%c[38;2;%d;%d;%d;48;2;%d;%d;%dm", ESC, fg_r, fg_g, fg_b, bg_r, bg_g, bg_b);
    }

    void SetColor24bit(uint32_t fg_rgb, uint32_t bg_rgb)
    {
        std::printf("%c[38;2;%d;%d;%d;48;2;%d;%d;%dm", ESC,
            fg_rgb / (uint32_t)0x00'01'00'00 % 0x1'00,
            fg_rgb / (uint32_t)0x00'00'01'00 % 0x1'00,
            fg_rgb / (uint32_t)0x00'00'00'01 % 0x1'00,
            bg_rgb / (uint32_t)0x00'01'00'00 % 0x1'00,
            bg_rgb / (uint32_t)0x00'00'01'00 % 0x1'00,
            bg_rgb / (uint32_t)0x00'00'00'01 % 0x1'00
        );
    }

    void ResetColor()
    {
        std::printf("%c[0m", ESC);
    }

    bool UpdateSize()
    {
        struct winsize size;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) != 0)
            return false;
            
        size_x = size.ws_col;
        size_y = size.ws_row;

        return true;
    }

    int16_t GetSizeX()
    {
        return size_x;
    }

    int16_t GetSizeY()
    {
        return size_y;
    }

    void GetSize(int16_t& x, int16_t& y)
    {
        x = size_x;
        y = size_y;
    }

    void HideCursor()
    {
        std::printf("%c[?25l", ESC);
    }

    void ShowCursor()
    {
        std::printf("%c[?25h", ESC);
    }

    bool GetInputBuf(std::string& buf)
    {
        termios term;
        tcflag_t prev_lflag;

        tcgetattr(STDIN_FILENO, &term);
        prev_lflag = term.c_lflag;

        term.c_lflag &= ~(ICANON | ECHO);
        term.c_cc[VTIME] = 0;
        term.c_cc[VMIN] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

        // number of bytes to be read
        int bytes_read;
        if (ioctl(STDIN_FILENO, FIONREAD, &bytes_read) != 0)
            return false;

        // read the inputs
        char* c_buf = (char*)malloc((bytes_read) * sizeof(char));
        if (read(STDIN_FILENO, c_buf, bytes_read) < 0)
            return false;
        
        buf = std::string(c_buf, bytes_read);

        // restore terminal attributes
        term.c_lflag = prev_lflag;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

        return true;
    }

    bool PollEvent(Event& event)
    {
        if (!event_queue.empty())
        {
            event = event_queue.front();
            event_queue.pop_front();

            return true;
        }

        // system
        if ((signal_status & Signal::Exit) != 0)
        {
            event_queue.emplace_back(Event
            {
                .type = Event::System,
                .system = SystemEvent{ .type = SystemEvent::Exit },
            });
        }

        if ((signal_status & Signal::Resize) != 0)
        {
            UpdateSize();
            event_queue.emplace_back(Event
            {
                .type = Event::System,
                .system = SystemEvent{ .type = SystemEvent::Resize },
            });
        }

        signal_status = Signal::None;

        // input
        std::string buf;
        if (!GetInputBuf(buf) || buf.empty())
            return false;

        for (size_t i = 0; i < buf.size(); ++i)
        {
            if (buf.at(i) == ESC)
            {
                uint8_t code;
                bool is_alt;
                int seq_len = MapEscSeqToInputCode(buf.c_str() + i, buf.size() - i, code, is_alt);
                
                if (seq_len == 0)
                    continue;

                if (seq_len == 1)
                {
                    event_queue.emplace_back(Event
                    {
                        .type = Event::Input,
                        .input = InputEvent
                        {
                            .code = InputEvent::Escape,
                            .is_esc = false,
                            .is_alt = false,
                        },
                    });

                    continue;
                }

                i += seq_len - 1;

                event_queue.emplace_back(Event
                {
                    .type = Event::Input,
                    .input = InputEvent
                    {
                        .code = code,
                        .is_esc = !is_alt,
                        .is_alt = is_alt,
                    },
                });
            }
            else
            {
                event_queue.emplace_back(Event
                {
                    .type = Event::Input,
                    .input = InputEvent
                    {
                        .code = (uint8_t)buf.at(i),
                        .is_esc = false,
                        .is_alt = false,
                    },
                });
            }
        }

        // pop event
        if (!event_queue.empty())
        {
            event = event_queue.front();
            event_queue.pop_front();

            return true;
        }

        return false;
    }

    namespace
    {
        void SAHandler(int32_t signal)
        {
            switch (signal)
            {
                case SIGWINCH:  signal_status |= Signal::Resize; return;
                case SIGINT:    signal_status |= Signal::Exit; return;

                default: return;
            }
        }

        int32_t MapEscSeqToInputCode(const char* esc_seq, int32_t size, uint8_t& code, bool& is_alt)
        {
            if (size < 1 || esc_seq[0] != ESC)
                return 0;

            is_alt = false;
            
            if (size < 2)
                return 1;
            
            if (size > 2)
            {
                if (esc_seq[1] == '1' || esc_seq[1] == 'O')
                {
                    if      (esc_seq[2] == 'P') { code = InputEvent::F1; return 3; }
                    else if (esc_seq[2] == 'Q') { code = InputEvent::F2; return 3; }
                    else if (esc_seq[2] == 'R') { code = InputEvent::F3; return 3; }
                    else if (esc_seq[2] == 'S') { code = InputEvent::F4; return 3; }
                }
                else if (esc_seq[1] == '[')
                {
                    if (size > 3)
                    {
                        if (esc_seq[3] == '~')
                        {
                            if      (esc_seq[2] == '1')     { code = InputEvent::Home;      return 4; }
                            else if (esc_seq[2] == '2')     { code = InputEvent::Insert;    return 4; }
                            else if (esc_seq[2] == '3')     { code = InputEvent::Delete;    return 4; }
                            else if (esc_seq[2] == '4')     { code = InputEvent::End;       return 4; }
                            else if (esc_seq[2] == '5')     { code = InputEvent::PgUp;      return 4; }
                            else if (esc_seq[2] == '6')     { code = InputEvent::PgDown;    return 4; }
                            else if (esc_seq[2] == '7')     { code = InputEvent::Home;      return 4; }
                            else if (esc_seq[2] == '8')     { code = InputEvent::End;       return 4; }
                        }
                        
                        if (size > 4 && esc_seq[4] == '~')
                        {
                            if      (esc_seq[2] == '1' && esc_seq[3] == '0')    { code = InputEvent::F0;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '1')    { code = InputEvent::F1;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '2')    { code = InputEvent::F2;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '3')    { code = InputEvent::F3;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '4')    { code = InputEvent::F4;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '5')    { code = InputEvent::F5;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '7')    { code = InputEvent::F6;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '8')    { code = InputEvent::F7;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '9')    { code = InputEvent::F8;        return 5; }
                        }
                    }

                    if      (esc_seq[2] == 'A') { code = InputEvent::Up;    return 3; }
                    else if (esc_seq[2] == 'B') { code = InputEvent::Down;  return 3; }
                    else if (esc_seq[2] == 'C') { code = InputEvent::Right; return 3; }
                    else if (esc_seq[2] == 'D') { code = InputEvent::Left;  return 3; }
                    else if (esc_seq[2] == 'F') { code = InputEvent::End;   return 3; }
                    else if (esc_seq[2] == 'H') { code = InputEvent::Home;  return 3; }
                }
            }
            
            if (std::isprint(esc_seq[1]))
            {
                is_alt = true;
                code = esc_seq[1];
                return 2;
            }

            return 1;
        }
    }
}