#pragma once
#include <ncurses.h>
#include "../chess/game.hpp"

class BoardView {

public:

    explicit BoardView(WINDOW* window);

    void draw();
    void clear();
    void setCursor(int row, int col);

    int getCursorRow() const { return cursor_row; }
    int getCursorCol() const { return cursor_col; }

    void setGameManager(const Chess::GameManager* g) { game = g; }

private:

    WINDOW* window;
    const Chess::GameManager* game = nullptr;

    int cursor_row = 0;
    int cursor_col = 0;

    void drawSquare(int row, int col);
};
