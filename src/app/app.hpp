#pragma once
#include <ncurses.h>
#include <memory>

#include "../ui/layout.hpp"
#include "../ui/boardview.hpp"
#include "../ui/moveview.hpp"
#include "../ui/status.hpp"
#include "../chess/game.hpp"

class App {

public:

    App();
    void run();

private:
    Layout layout;
    Chess::GameManager game;

    int cursor_row = 0;
    int cursor_col = 0;

    std::unique_ptr<BoardView> board;
    std::unique_ptr<MoveView> moves;
    std::unique_ptr<StatusBar> status;

    void handleInput(int ch);
    void updateUI();

};
