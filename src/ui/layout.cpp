#include "layout.hpp"
#include "../app/config.hpp"

Layout::Layout() {
    getmaxyx(stdscr, term_rows, term_cols);

    init_pair(13, COLOR_BLUE, -1);
    createWindows();
}

Layout::~Layout() {
    destroyWindows();
}

void Layout::resize() {
    int new_rows, new_cols;
    getmaxyx(stdscr, new_rows, new_cols);

    if (new_rows != term_rows || new_cols != term_cols) {
        term_rows = new_rows;
        term_cols = new_cols;
        destroyWindows();
        createWindows();
    }
}

void Layout::createWindows() {

    const int board_height = 8 * (SQUARE_HEIGHT) + 4;
    const int board_width  = 8 * (SQUARE_WIDTH) + 6;

    const int board_y = (term_rows - board_height - SQUARE_HEIGHT) / 2;
    const int board_x = 1;

    board_win = newwin(board_height, board_width, board_y, board_x);

    const std::string boardBinds = "H J K L ═ ← ↓ ↑ → ┗━┛ ⏎ or ␣ de/select";
    drawBorder(board_win, 13, "Board", boardBinds);

    const int moves_height = term_rows - 3;
    const int moves_width  = term_cols - board_width - 6;

    const int moves_y = 1;
    const int moves_x = board_x + board_width + 3;

    moves_win = newwin(moves_height, moves_width, moves_y, moves_x);

    const std::string moveBinds = " up down - scroll ";
    drawBorder(moves_win, 13, "Moves", moveBinds);

    const int status_height = 1;
    const int status_width  = term_cols;
    const int status_y      = term_rows - 1;
    const int status_x      = 0;

    status_win = newwin(status_height, status_width, status_y, status_x);

    wrefresh(board_win);
    wrefresh(moves_win);
    wrefresh(status_win);
}

void Layout::destroyWindows() {
    if (board_win) {
        delwin(board_win);
        board_win = nullptr;
    }

    if (moves_win) {
        delwin(moves_win);
        moves_win = nullptr;
    }

    if (status_win) {
        delwin(status_win);
        status_win = nullptr;
    }
}

void Layout::drawBorder(WINDOW* win, short color_pair, const std::string& title, const std::string& extra) {
    int h, w;
    getmaxyx(win, h, w);

    // Prepare wide characters
    cchar_t ch_tl, ch_tr, ch_bl, ch_br;
    cchar_t ch_h, ch_v;

    setcchar(&ch_tl, L"┏", 0, color_pair, nullptr);
    setcchar(&ch_tr, L"┓", 0, color_pair, nullptr);
    setcchar(&ch_bl, L"┗", 0, color_pair, nullptr);
    setcchar(&ch_br, L"┛", 0, color_pair, nullptr);

    setcchar(&ch_h, L"━", 0, color_pair, nullptr);
    setcchar(&ch_v, L"┃", 0, color_pair, nullptr);

    // Draw corners
    mvwadd_wch(win, 0, 0, &ch_tl);
    mvwadd_wch(win, 0, w - 1, &ch_tr);
    mvwadd_wch(win, h - 1, 0, &ch_bl);
    mvwadd_wch(win, h - 1, w - 1, &ch_br);

    // Draw top and bottom edges
    for (int x = 1; x < w - 1; x++) {
        mvwadd_wch(win, 0, x, &ch_h);
        mvwadd_wch(win, h - 1, x, &ch_h);
    }

    // Draw left and right edges
    for (int y = 1; y < h - 1; y++) {
        mvwadd_wch(win, y, 0, &ch_v);
        mvwadd_wch(win, y, w - 1, &ch_v);
    }

    if (!title.empty()) {
        std::string title_new = "┓ " + title + " ┏";

        int start_x = 4;

        wattron(win, COLOR_PAIR(color_pair));
        mvwprintw(win, 0, start_x, "%s", title_new.c_str());
        wattroff(win, COLOR_PAIR(color_pair));
    }

    if (!extra.empty()) {
        std::string extra_new = "┛ " + extra + " ┗";

        int start_x = w - extra_new.length();
        if (title == "Board")
            start_x += 10;

        wattron(win, COLOR_PAIR(color_pair));
        mvwprintw(win, h - 1, start_x, "%s", extra_new.c_str());
        wattroff(win, COLOR_PAIR(color_pair));
    }
}

void Layout::draw() {
    drawBorder(board_win, 13, "Board", "H J K L ═ ← ↓ ↑ → ┗━┛ ⏎ or ␣ de/select");
    drawBorder(moves_win, 13, "Moves", " up down - scroll ");
    drawBorder(status_win, 13, "", "");
}
