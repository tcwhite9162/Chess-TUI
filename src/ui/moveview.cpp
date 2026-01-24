#include "moveview.hpp"
#include <ncurses.h>

MoveView::MoveView(WINDOW* window) : window(window) {}

void MoveView::addMove(const std::string& move) {
    moves.push_back(move);

    if ((int)moves.size() > scroll_pos + getmaxy(window) - 2) {
        scroll_pos = moves.size() - (getmaxy(window) - 2);

        if (scroll_pos < 0)
            scroll_pos = 0;
    }
}

void MoveView::clearMoves() {
    moves.clear();
    scroll_pos = 0;
}

void MoveView::scrollUp() {
    if (scroll_pos > 0)
        scroll_pos--;
}

void MoveView::scrollDown() {
    int lines = getmaxy(window) - 2;
    if (scroll_pos + lines < (int)moves.size()) {
        scroll_pos++;
    }
}

void MoveView::draw() {
    int h, w;
    getmaxyx(window, h, w);

    for (int y = 1; y < h- 1; y++) {
        for (int x = 1; x < w-1; x++) {
            mvwaddch(window, y, x, ' ');
        }
    }

    drawMoves();
    wrefresh(window);
}

void MoveView::drawMoves() {
    int max_y = getmaxy(window);

    int visible = max_y - 2;
    int start = scroll_pos;
    int end = std::min(start+visible, (int)moves.size());

    for (int i = start; i < end; i++) {
        int line = i - start + 1;
        mvwprintw(window, line, 1, "%s", moves[i].c_str());
    }
}
