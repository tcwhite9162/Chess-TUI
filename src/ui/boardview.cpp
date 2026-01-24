#include "boardview.hpp"
#include "../app/config.hpp"
#include <ncurses.h>

BoardView::BoardView(WINDOW* window) : window(window) {
    start_color();
    use_default_colors();

    if (can_change_color()) {
        init_color(10, 900, 900, 900); // light square
        init_color(11, 200, 200, 300); // dark square
        init_color(12, 300, 700, 800); // cursor
        init_color(13, 100, 100, 200); // border
    }

    init_pair(1, COLOR_BLACK, 10);
    init_pair(2, COLOR_BLACK, 11);
    init_pair(3, COLOR_BLACK, 12);
    init_pair(4, COLOR_BLACK, 13);
}

void BoardView::setCursor(int row, int col) {
    cursor_row = row;
    cursor_col = col;
}

void BoardView::draw() {
    int h, w;
    getmaxyx(window, h, w);

    for (int y = 1; y < h-1; y++) {
        for (int x = 1; x < w-1; x++) {
            mvwaddch(window, y, x, ' ');
        }
    }

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            drawSquare(r , c);
        }
    }

    wrefresh(window);
}

void BoardView::clear() {
    werase(window);
    box(window, 0, 0);
    wrefresh(window);
}

void BoardView::drawSquare(int row, int col) {
    const int y = row * SQUARE_HEIGHT + 2;
    const int x = col * SQUARE_WIDTH  + 3;

    const bool is_dark = (row + col) % 2 == 1;

    const bool is_cursor = (row == cursor_row && col == cursor_col);

    int color_pair = 0;
    if (has_colors()) {
        if (is_cursor)
            color_pair = 3;
        else if (is_dark)
            color_pair = 2;
        else
            color_pair = 1;

        wattron(window, COLOR_PAIR(color_pair));
    }

    for (int dy = 0; dy < SQUARE_HEIGHT; dy++) {
        for (int dx = 0; dx < SQUARE_WIDTH; dx++) {
            mvwaddch(window, y+dy, x+dx, ' ');
        }
    }

    if (has_colors())
        wattroff(window, COLOR_PAIR(color_pair));
}
