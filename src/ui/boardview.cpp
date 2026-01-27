#include "boardview.hpp"
#include "../app/config.hpp"
#include <ncurses.h>
//#include <iostream>

static const char* pieceToString(Chess::Piece p) {
    switch (p) {
        case Chess::W_PAWN:   return "WP";
        case Chess::W_KNIGHT: return "WN";
        case Chess::W_BISHOP: return "WB";
        case Chess::W_ROOK:   return "WR";
        case Chess::W_QUEEN:  return "WQ";
        case Chess::W_KING:   return "WK";

        case Chess::B_PAWN:   return "BP";
        case Chess::B_KNIGHT: return "BN";
        case Chess::B_BISHOP: return "BB";
        case Chess::B_ROOK:   return "BR";
        case Chess::B_QUEEN:  return "BQ";
        case Chess::B_KING:   return "BK";
        default:      return "  "; // empty square
    }
}

BoardView::BoardView(WINDOW* window) : window(window) {
    start_color();
    use_default_colors();

    if (can_change_color()) {
        init_color(110, 900, 900, 900); // light square
        init_color(111, 200, 200, 300); // dark square
        init_color(112, 800, 700, 950); // cursor
        init_color(113, 900, 600, 500); // selected square
        init_color(114, 0, 0, 0);          // black
        init_color(115, 1000, 1000, 1000); // white
        init_color(215, 1000, 0, 1000); // legal move

    }

    init_pair(1, 114, 110);
    init_pair(2, 115, 111);
    init_pair(3, COLOR_BLACK, 112);
    init_pair(4, COLOR_BLACK, 113);
    init_pair(14, COLOR_BLACK, 215);
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

    int boardRow = 7 - row;   // flip
    int boardCol = col;

    const bool is_dark = (row + col) % 2 == 1;
    const bool is_cursor = (row == cursor_row && col == cursor_col);

    bool is_selected = false;
    bool is_legal = false;
    if (game) {
        is_selected = game->isSelected(boardRow, boardCol);
        is_legal = game->isLegalSq(boardRow, boardCol);
    }

    int color_pair = 0;
    if (has_colors()) {
        if (is_selected)
            color_pair = 4;
        else if (is_cursor)
            color_pair = 3;
        else if (is_dark)
            color_pair = 2;
        else
            color_pair = 1;

        wattron(window, COLOR_PAIR(color_pair));
    }

    for (int dy = 0; dy < SQUARE_HEIGHT; dy++)
        for (int dx = 0; dx < SQUARE_WIDTH; dx++)
            mvwaddch(window, y+dy, x+dx, ' ');

    Chess::Piece p = game->pieceAt(boardRow, boardCol);
    const char* label = pieceToString(p);

    int text_y = y + SQUARE_HEIGHT / 2;
    int text_x = x + (SQUARE_WIDTH - 2) / 2;

    wattron(window, COLOR_PAIR(color_pair) | A_BOLD);
    mvwprintw(window, text_y, text_x, "%s", label);
    wattroff(window, COLOR_PAIR(color_pair) | A_BOLD);

    // Draw legal move indicator (small dot)
    if (is_legal && !is_selected) {
        int dot_y = y + SQUARE_HEIGHT / 2;
        int dot_x = x + SQUARE_WIDTH  / 2;

        wattron(window, COLOR_PAIR(14) | A_BOLD);
        mvwaddch(window, dot_y, dot_x, ' ');   // or ACS_BULLET
        mvwaddch(window, dot_y, dot_x-1, ' ');   // or ACS_BULLET
        mvwaddch(window, dot_y-1, dot_x, ' ');   // or ACS_BULLET
        mvwaddch(window, dot_y-1, dot_x-1, ' ');   // or ACS_BULLET
        wattroff(window, COLOR_PAIR(14) | A_BOLD);
    }
}
