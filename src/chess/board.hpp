#pragma once

#include <array>
#include <vector>

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

    [[nodiscard]] inline Piece pieceAt(int sq)       const { return pieces[sq]; }
    [[nodiscard]] inline Piece pieceAt(int r, int c) const { return pieces[r * 8 + c]; }

    [[nodiscard]] inline Color getTurn()         const { return sideToMove; }
    [[nodiscard]] inline int getEnPassantSq()    const { return enPassantSquare; }
    [[nodiscard]] inline int getCastlingRights() const { return castlingRights; }

    [[nodiscard]] inline bool isWhite(Piece p) const { return p > 0; }
    [[nodiscard]] inline bool isBlack(Piece p) const { return p < 0; }

    [[nodiscard]] inline bool canWhiteCastleKingside()  const { return castlingRights & CastlingRights::WK; }
    [[nodiscard]] inline bool canWhiteCastleQueenside() const { return castlingRights & CastlingRights::WQ; }
    [[nodiscard]] inline bool canBlackCastleKingside()  const { return castlingRights & CastlingRights::BK; }
    [[nodiscard]] inline bool canBlackCastleQueenside() const { return castlingRights & CastlingRights::BQ; }

    inline void setCastlingRights(int r) { castlingRights = r; }
    inline void flipTurn()               { sideToMove = (sideToMove == WHITE ? BLACK : WHITE); }
    inline void setEnPassantSq(int sq)   { enPassantSquare = sq; }

    inline void setPiece(int sq, Piece p) { pieces[sq] = p; }
    inline void removePiece(int sq)       { pieces[sq] = EMPTY; }

    inline void setPiece(int r, int c, Piece p) { pieces[r * 8 + c] = p; }
    inline void removePiece(int r, int c)       { pieces[r * 8 + c] = EMPTY; }

    UndoState makeMove(const Move& move);
    void unmakeMove(const Move& move, const UndoState& undo);

    void generateLegalMoves(std::vector<Move>& moves);

    [[nodiscard]] bool isSquareAttacked(const int sq, Color attacking) const;
    [[nodiscard]] bool isInCheck(Color side) const;

private:
    std::array<Piece, NUM_SQUARES> pieces;

    Color sideToMove;
    int castlingRights;
    int enPassantSquare;
    int whiteKingPos;
    int blackKingPos;

    void generatePseudoLegalMoves(std::vector<Move>& moves) const;

    void genPawnMoves(std::vector<Move>& moves, const int sq) const;
    void genKnightMoves(std::vector<Move>& moves, const int sq) const;
    void genBishopMoves(std::vector<Move>& moves, const int sq) const;
    void genRookMoves(std::vector<Move>& moves, const int sq) const;
    void genQueenMoves(std::vector<Move>& moves, const int sq) const;
    void genKingMoves(std::vector<Move>& moves, const int sq) const;

    void genSlidingMoves(std::vector<Move>& moves, const int sq, const int* directions, const int dirCount) const;
};

}
