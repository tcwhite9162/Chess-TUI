#pragma once
#include <ncurses.h>
#include <vector>
#include <string>

class MoveView {

public:

    explicit MoveView(WINDOW* window);

    void addMove(const std::string& move);
    void clearMoves();

    void draw();
    void scrollUp();
    void scrollDown();

private:

    WINDOW* window;

    std::vector<std::string> moves;
    int scroll_pos = 0;

    void drawMoves();

};
