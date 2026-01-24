#include <ncurses.h>

#include "app/app.hpp"

int main() {
    setlocale(LC_ALL, ""); // for utf 8 chars
    initscr();
    start_color();
    use_default_colors();
    refresh();

    noecho();
    keypad(stdscr, TRUE);

    App app;
    app.run();

    endwin();

    return 0;
}
