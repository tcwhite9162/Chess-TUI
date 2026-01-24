#pragma once
#include <ncurses.h>
#include <string>

class StatusBar {

public:

    explicit StatusBar(WINDOW* window);

    void setMessage(const std::string& msg);
    void clearMessage();
    void draw();

private:

    WINDOW* window;
    std::string message;
    std::string bindsStr = " ━━ q, quit ━━ ";

};
