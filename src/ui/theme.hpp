#pragma once

#include <ncurses.h>
#include <vector>
#include <string>

class ThemeManager {

public:
    explicit ThemeManager(WINDOW* parent);

    void open();
    void close();

    void draw();

    void handleInput(const int ch);
    void addTheme(const std::string& theme);

    inline bool isOpen() const { return is_open; };
    inline int getTheme() const { return cursor; };

    void toggle();

private:
    WINDOW* parent;
    WINDOW* popup;

    int currentTheme;

    bool is_open = false;
    int cursor = 0;
    int width, height;

    std::vector<std::string> themes;

    void createPopup();
    void destroyPopup();
    void drawPopup();
};
