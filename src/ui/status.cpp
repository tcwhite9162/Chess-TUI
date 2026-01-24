#include "status.hpp"

StatusBar::StatusBar(WINDOW* window) : window(window) {}

void StatusBar::setMessage(const std::string& msg) {
    message = msg;
}

void StatusBar::clearMessage() {
    message.clear();
}

void StatusBar::draw() {
    werase(window);

    if (!message.empty())
        mvwprintw(window, 0, 1, "%s", message.c_str());

    wrefresh(window);
}
