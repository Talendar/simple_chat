#include "window.h"

#define INPUT_WIN_H 5

Window::Window() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();

    // Get information from terminal
    int width, height;
    getmaxyx(stdscr, height, width);

    // Set message log window box
    msg_win_box = newwin(height-INPUT_WIN_H, width, 0, 0);
    refresh();

    // Set message box in window
    box(msg_win_box, 0, 0);
    wrefresh(msg_win_box);

    // Set message log window 
    msg_win = subwin(msg_win_box,height-INPUT_WIN_H-2,width-2,1,1);
    scrollok(msg_win,true);

    // Set input window
    input_win = newwin(INPUT_WIN_H, width, height-INPUT_WIN_H, 0);
    refresh();
    wrefresh(input_win);

    // Set input box in window
    box(input_win, 0, 0);
    wrefresh(input_win);
}

Window::~Window() {
    endwin();
}

std::string Window::get_message() {
    std::string message;
    char c;
    while((c = wgetch(input_win)) != '\n') {
        // Avoid backspace
        if(c != '\b' && c != 127)
            message += c;
        else if(message.length() > 0){
            message.pop_back();
            clear_input();
        }
        mvwprintw(input_win, 1, 1, "%s", message.c_str());
    }
    return message;
}

void Window::log_message(const char *message) {
    wprintw(msg_win, "%s\n", message);
    wrefresh(msg_win);
}

void Window::clear_input() {
    wclear(input_win);
    box(input_win, 0, 0);
    wrefresh(input_win);
}