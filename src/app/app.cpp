#include "app.hpp"
#include <memory>
#include <ncurses.h>
#include <string>

App::App() {
    board = std::make_unique<BoardView>(layout.boardWindow());
    moves = std::make_unique<MoveView>(layout.movesWindow());
    status = std::make_unique<StatusBar>(layout.statusWindow());

    board->setGameManager(&game);

    std::string msg;
    msg = std::to_string(cursor_row + 1)+ ", " + std::to_string(cursor_col + 1);
    status->setMessage(msg);
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
    std::string msg;

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

        case '\n':
        case ' ':
            game.selectSquare(7-cursor_row, cursor_col);
            break;
    }

    msg = std::to_string(cursor_row + 1)+ ", " + std::to_string(cursor_col + 1);
    status->setMessage(msg);
}

void App::updateUI() {
    board->setCursor(cursor_row, cursor_col);
    board->draw();

    moves->draw();
    status->draw();
}
