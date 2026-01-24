#pragma once

#include <ncurses.h>
#include <string>

class Layout {

public:

    Layout();
    ~Layout();

    void resize();

    WINDOW* boardWindow()  { return board_win; }
    WINDOW* movesWindow()  { return moves_win; }
    WINDOW* statusWindow() { return status_win; }

private:

    void createWindows();
    void destroyWindows();

    WINDOW* board_win  = nullptr;
    WINDOW* moves_win  = nullptr;
    WINDOW* status_win = nullptr;

    int term_rows = 0;
    int term_cols = 0;

    void drawBorder(WINDOW* win, short color_pair, const std::string& title, const std::string& extra="");

};

