#pragma once
#include <string>

constexpr int SQUARE_HEIGHT = 4;
constexpr int SQUARE_WIDTH  = 8;

struct Color { int r, g, b; };

struct Theme {
    const std::string themeName;
    Color darkSq;
    Color lightSq;
    Color selectedSq;
    Color highlightSq;

    Color border;
    Color background;
    Color foreground;
};
