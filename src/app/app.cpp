#include "app.hpp"
#include <ncurses.h>
#include <string>

App::App() {
    board = new BoardView(layout.boardWindow());
    moves = new MoveView(layout.movesWindow());
    status = new StatusBar(layout.statusWindow());

    status->setMessage("hello");
}

void App::run() {
    int ch;

    updateUI();
    doupdate();

    while ((ch = getch()) != 'q') {
        handleInput(ch);
        updateUI();
        doupdate();
    }
}

void App::handleInput(int ch) {

    switch (ch) {
        case 'k':
            if (cursor_row > 0) 
                cursor_row--;
            break;

        case 'j':
            if (cursor_row < 7)
                cursor_row++;
            break;

        case 'h':
            if (cursor_col > 0)
                cursor_col--;
            break;

        case 'l':
            if (cursor_col < 7)
                cursor_col++;
            break;

        case KEY_UP:
            moves->scrollUp();
            break;

        case KEY_DOWN:
            moves->scrollDown();
            break;

        default:
            break;
    }

    std::string msg = "Cursor: " + std::to_string(board->getCursorRow()) + ", " + std::to_string(board->getCursorCol());
    status->setMessage(msg);
}

void App::updateUI() {
    board->setCursor(cursor_row, cursor_col);
    board->draw();
    moves->draw();
    status->draw();
}
