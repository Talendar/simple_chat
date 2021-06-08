#ifndef WINDOW_H
    #define WINDOW_H
    #include <ncurses.h>
    #include <string>

    class Window {
        private:
            WINDOW *msg_win;
            WINDOW *input_win;
        public:
            Window();
            ~Window();

            std::string get_message();
            void log_message(const char *message);
            void clear_input();
    };

#endif