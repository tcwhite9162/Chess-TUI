#pragma once

#include <array>

#include "move.hpp"
#include "constants.hpp"

namespace Chess
{

struct UndoState {
    Piece captured;
    int prevCastlingRights;
    int prevEnPassant;
    Color prevTurn;
};

class Board {

public:
    Board();
    void reset();

    void printBoard() const;

    [[nodiscard]] inline Piece pieceAt(int sq) const { return pieces[sq]; };
    [[nodiscard]] inline Piece pieceAt(int row, int col) const { return pieces[row * 8 + col]; };

    [[nodiscard]] inline Color getTurn() const { return sideToMove; }
    [[nodiscard]] inline int getEnPassantSq() const { return enPassantSquare; }
    [[nodiscard]] inline int getCastlingRights() const { return castlingRights; }

    [[nodiscard]] inline bool isWhite(Piece p) const { return p > 0; }
    [[nodiscard]] inline bool isBlack(Piece p) const { return p < 0; }

    void inline setCastlingRights(int rights) { castlingRights = rights; }
    void inline flipTurn() { sideToMove = (sideToMove == WHITE ? BLACK : WHITE); }
    void inline setEnPassantSq(int sq) { enPassantSquare = sq; }

    inline void setPiece(int sq, Piece piece) { pieces[sq] = piece; }
    inline void removePiece(int sq) { pieces[sq] = EMPTY; }

    inline void setPiece(int row, int col, Piece piece) { pieces[row * 8 + col] = piece; }
    inline void removePiece(int row, int col) { pieces[row * 8 + col] = EMPTY; }

    UndoState makeMove(const Move& m);
    void unmakeMove(const Move& move, const UndoState& undo);

private:
    std::array<Piece, NUM_SQUARES> pieces;

    Color sideToMove;
    int castlingRights;
    int enPassantSquare;
};

}
