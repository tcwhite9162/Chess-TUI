#include "status.hpp"
#include <ncurses.h>

StatusBar::StatusBar(WINDOW* window) : window(window) {}

void StatusBar::setMessage(const std::string& msg) {
    message = msg;
}

void StatusBar::clearMessage() {
    message.clear();
}

void StatusBar::draw() {
    werase(window);
    int w = getmaxx(window);

    if (!message.empty())
        mvwprintw(window, 0, 1, "%s", message.c_str());

    mvwprintw(window, 0, w - bindsStr.size(), "%s", bindsStr.c_str());

    wrefresh(window);
}
