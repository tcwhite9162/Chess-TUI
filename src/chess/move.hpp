#pragma once

#include <string>
#include <optional>

#include "constants.hpp"

namespace Chess {

inline char fileChar(const int col) { return 'a' + col; }
inline char rankChar(const int row) { return '1' + (7 - row); }

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

    std::optional<char> promotionChar() const {
        switch (promotion) {
            case W_KNIGHT:
            case B_KNIGHT:
                return 'n';
            case W_BISHOP:
            case B_BISHOP:
                return 'b';
            case W_ROOK:
            case B_ROOK:
                return 'r';
            case W_QUEEN:
            case B_QUEEN:
                return 'q';
            default:
                return std::nullopt;
        }
    }

    std::string toUci() const {
        std::string s;

        s += fileChar(fromCol());
        s += rankChar(fromRow());
        s += fileChar(toCol());
        s += rankChar(toRow());

        if (isPromotion()) {
            auto p = promotionChar();
            if (p.has_value())
                s += p.value();
        }

        return s;
    }

};

}
