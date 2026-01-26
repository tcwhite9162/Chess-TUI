#include "app.hpp"
#include "../chess/constants.hpp"
#include <memory>
#include <ncurses.h>
#include <string>

App::App() {
    board = std::make_unique<BoardView>(layout.boardWindow());
    moves = std::make_unique<MoveView>(layout.movesWindow());
    status = std::make_unique<StatusBar>(layout.statusWindow());
    themeManager  = std::make_unique<ThemeManager>(stdscr);

    board->setGameManager(&game);

    themeManager->addTheme("Classic");
    themeManager->addTheme("Tokyo Night");
    themeManager->addTheme("Dracula");

    std::string msg;
    std::string to_move = (game.getTurn() == Chess::WHITE) ? "White to move" : "Black to move";
    msg = std::to_string(cursor_row + 1)+ ", " + std::to_string(cursor_col + 1) + " --- " + to_move;
    status->setMessage(msg);
}

void App::run() {
    int ch;

    updateUI();
    doupdate();

    while (running) {

        ch = getch();
        handleInput(ch);
        updateUI();
        doupdate();
    }
}

void App::handleInput(int ch) {
    if (themeManager->isOpen()) {
        themeManager->handleInput(ch);

        if (!themeManager->isOpen()) {
            int idx = themeManager->getTheme();
            idx++;
            // TODO: applyTheme(idx);
        }

        return;
    }

    std::string msg;
    switch (ch) {
        case 't':
            themeManager->toggle();
            return;

        case 'q':
            running = false;
            break;

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

    std::string to_move = (game.getTurn() == Chess::WHITE) ? "White to move" : "Black to move";
    msg = std::to_string(cursor_row + 1)+ ", " + std::to_string(cursor_col + 1) + " --- " + to_move;
    status->setMessage(msg);
}

void App::updateUI() {
    layout.draw();

    board->setCursor(cursor_row, cursor_col);
    board->draw();

    moves->draw();
    status->draw();

    themeManager->draw();
}
