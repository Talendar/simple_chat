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

    // Set message box in window
    box(msg_win_box, 0, 0);

    // Set message log window 
    msg_win = subwin(msg_win_box,height-INPUT_WIN_H-2,width-2,1,1);
    scrollok(msg_win,true);

    // Set message log window box
    input_win_box = newwin(INPUT_WIN_H, width, height-INPUT_WIN_H, 0);

    // Set message box in window
    box(input_win_box, 0, 0);

    // Set input window
    input_win = subwin(input_win_box, INPUT_WIN_H-2, width-2, height-INPUT_WIN_H+1, 1);
    scrollok(input_win,true);
    refresh();
    wrefresh(msg_win_box);
    wrefresh(msg_win);
    wrefresh(input_win_box);
    wrefresh(input_win);
}

Window::~Window() {
    endwin();
}

std::string Window::get_message() {
    std::string message;
    char c;
    // Display typed characters
    while((c = wgetch(input_win)) != '\n') {
        // Avoid backspace
        if(c != '\b' && c != 127)
            message += c;
        else if(message.length() > 0){
            message.pop_back();
            clear_input();
        }
        mvwprintw(input_win, 0, 0, "%s", message.c_str());
    }
    // Check if there's an escape code
    if(message.find('\x1b') != std::string::npos) {
        return std::string();
    }
    return message;
}

void Window::log_message(const char *message) {
    wprintw(msg_win, "%s\n", message);
    wrefresh(msg_win);
}

void Window::clear_input() {
    wclear(input_win);
    wrefresh(input_win);
}