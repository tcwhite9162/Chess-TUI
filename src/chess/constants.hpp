#pragma once

namespace Chess
{

constexpr int BOARD_SIZE = 8;
constexpr int NUM_SQUARES = 64;

enum Color : int {
    WHITE = 0,
    BLACK = 1,
    NO_COLOR  = -1
};

enum Piece : int {
    B_KING = -6,
    B_QUEEN = -5,
    B_ROOK = -4,
    B_BISHOP = -3,
    B_KNIGHT = -2,
    B_PAWN = -1,

    EMPTY = 0,

    W_PAWN = 1,
    W_KNIGHT = 2,
    W_BISHOP = 3,
    W_ROOK = 4,
    W_QUEEN = 5,
    W_KING = 6
};

enum Flag : int {
    NONE = 0,
    DOUBLE_PUSH  = 1 << 0,
    CAPTURE      = 1 << 1,
    PROMOTION    = 1 << 2,
    EN_PASSANT   = 1 << 3,
    CASTLE_KING  = 1 << 4,
    CASTLE_QUEEN = 1 << 5,
};

enum CastlingRights : int {
    WK = 1 << 0,
    WQ = 1 << 1,
    BK = 1 << 2,
    BQ = 1 << 3
};

}
