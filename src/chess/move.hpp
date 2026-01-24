#pragma once
#include "constants.hpp"

namespace Chess {

struct Move {
    int from;
    int to;
    Piece promotion;
    int flags;

    Move(int f=-1, int t=-1, Piece p=EMPTY, int fl=NONE)
        : from(f), to(t), promotion(p), flags(fl) {}

    [[nodiscard]] bool isCapture()     const { return flags & Flag::CAPTURE; }
    [[nodiscard]] bool isPromotion()   const { return flags & Flag::PROMOTION; }
    [[nodiscard]] bool isEnPassant()   const { return flags & Flag::EN_PASSANT; }
    [[nodiscard]] bool isDoublePush()  const { return flags & Flag::DOUBLE_PUSH; }
    [[nodiscard]] bool isCastleK()     const { return flags & Flag::CASTLE_KING; }
    [[nodiscard]] bool isCastleQ()     const { return flags & Flag::CASTLE_QUEEN; }

    [[nodiscard]] int toRow() const { return to / 8; }
    [[nodiscard]] int toCol() const { return to % 8; }
    [[nodiscard]] int fromRow() const { return from / 8; } 
    [[nodiscard]] int fromCol() const { return from % 8; } 

    bool isValid() const { return from >= 0 && from < 64 && to >= 0 && to < 64; }

};

}
