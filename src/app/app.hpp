#pragma once

#include <memory>
#include <ncurses.h>

#include "../chess/game.hpp"
#include "../ui/boardview.hpp"
#include "../ui/layout.hpp"
#include "../ui/moveview.hpp"
#include "../ui/status.hpp"
#include "../ui/theme.hpp"

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
    std::unique_ptr<ThemeManager> themeManager;

    void handleInput(int ch);
    void updateUI();

    bool running = true;
};
