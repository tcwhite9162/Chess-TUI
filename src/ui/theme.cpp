#include "theme.hpp"
#include <ncurses.h>

ThemeManager::ThemeManager(WINDOW* parent)
    : parent(parent), popup(nullptr) {}

void ThemeManager::open() {
    if (!is_open) {
        is_open = true;
        createPopup();
    }
}

void ThemeManager::close() {
    if (is_open) {
        is_open = false;
        destroyPopup();
    }
}

void ThemeManager::toggle() {
    if (is_open) {
        close();
        currentTheme = cursor;
        cursor = 0;
    }
    else {
        open();
    }
}

void ThemeManager::createPopup() {
    int max_x, max_y;
    getmaxyx(parent, max_y, max_x);

    height = 6 + themes.size();
    width = 40;
    int y = (max_y - height) / 3;
    int x = (max_x - width) / 2;

    popup = newwin(height, width, y, x);
}

void ThemeManager::destroyPopup() {
    if (popup) {
        werase(popup);
        wrefresh(popup);
        delwin(popup);
        popup = NULL;
    }
}

void ThemeManager::handleInput(const int ch) {
    if (!is_open)
        return;

    switch (ch) {
        case '\n':
        case 'q':
        case 't':
            toggle();
            return;

        case 'k':
        case KEY_UP:
            cursor = (cursor + (int)themes.size()-1) % (int)themes.size();
            break;

        case 'j':
        case KEY_DOWN:
            cursor = (cursor+1) % (int)themes.size();
            break;
    }
}

void ThemeManager::draw() {
    if (!is_open || !popup)
        return;

    drawPopup();
}

void ThemeManager::addTheme(const std::string& theme) {
    themes.push_back(theme);
}

void ThemeManager::drawPopup() {
    init_color(116, 170, 200, 300);
    init_color(117, 1000, 800, 1000);
    init_pair(20, 117, 116);   // text black, background cyan

    werase(popup);

    // Apply popup background color
    wattron(popup, COLOR_PAIR(20));

    // Fill background with the color
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            mvwaddch(popup, y, x, ' ');
        }
    }

    // Draw border on top
    box(popup, 0, 0);

    // Title
    mvwprintw(popup, 0, 2, "Select Theme");

    // Theme list
    for (int i = 0; i < (int)themes.size(); i++) {
        if (i == cursor)
            wattron(popup, A_REVERSE);

        mvwprintw(popup, 2 + i, 2, "%s", themes[i].c_str());

        if (i == cursor)
            wattroff(popup, A_REVERSE);
    }

    wattroff(popup, COLOR_PAIR(20));
    wrefresh(popup);
}
