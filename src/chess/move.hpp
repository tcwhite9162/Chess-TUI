#pragma once
#include "constants.hpp"

namespace Chess {

struct Move {
    int from;
    int to;
    Piece promotion;
    int flags;

    Move(int f=-1, int t=-1, Piece p=EMPTY, int fl=NONE)
        : from(f), to(t), promotion(p), flags(f) {}

    bool isCapture()     const { return flags & Flag::CAPTURE; }
    bool isPromotion()   const { return flags & Flag::PROMOTION; }
    bool isEnPassant()   const { return flags & Flag::EN_PASSANT; }
    bool isDoublePush()  const { return flags & Flag::DOUBLE_PUSH; }
    bool isCastleK()     const { return flags & Flag::CASTLE_KING; }
    bool isCastleQ()     const { return flags & Flag::CASTLE_QUEEN; }

    bool isValid() const { return from >= 0 && from < 64 && to >= 0 && to < 64; }

};

}
