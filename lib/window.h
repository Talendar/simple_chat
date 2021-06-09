#ifndef WINDOW_H
    #define WINDOW_H
    #include <ncurses.h>
    #include <string>

    class Window {
        private:
            WINDOW *msg_win,*msg_win_box;
            WINDOW *input_win, *input_win_box;
        public:
            Window();
            ~Window();

            std::string get_message();
            void log_message(const char *message);
            void clear_input();
    };

#endif