#pragma once
#include <ncurses.h>

#include "../ui/layout.hpp"
#include "../ui/boardview.hpp"
#include "../ui/moveview.hpp"
#include "../ui/status.hpp"

class App {

public:

    App();
    void run();

private:

    Layout layout;

    BoardView* board  = nullptr;
    MoveView*  moves  = nullptr;
    StatusBar* status = nullptr;

    int cursor_row = 0;
    int cursor_col = 0;

    void handleInput(int ch);
    void updateUI();

};
