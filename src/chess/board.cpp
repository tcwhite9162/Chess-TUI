#include "board.hpp"
#include "constants.hpp"

namespace Chess {

Board::Board() {
    reset();
}

void Board::reset() {
    pieces.fill(EMPTY);

    for (int sq = 0; sq < 8; sq++) {
        pieces[sq+8]  = W_PAWN;
        pieces[sq+48] = B_PAWN;
    }

    pieces[0]  = W_ROOK; pieces[7]  = W_ROOK;
    pieces[56] = B_ROOK; pieces[63] = B_ROOK;

    pieces[1]  = W_KNIGHT; pieces[6]  = W_KNIGHT;
    pieces[57] = B_KNIGHT; pieces[62] = B_KNIGHT;

    pieces[2]  = W_BISHOP; pieces[5]  = W_BISHOP;
    pieces[58] = B_BISHOP; pieces[61] = B_BISHOP;

    pieces[3]  = W_QUEEN;
    pieces[59] = B_QUEEN;

    pieces[4]  = W_KING;
    pieces[60] = B_KING;

    sideToMove = WHITE;
    enPassantSquare = -1;
    castlingRights = WK | WQ | BK | BQ;
}

void Board::printBoard() const {
    return;
}

UndoState Board::makeMove(const Move& m) {
    UndoState undo;

    undo.prevCastlingRights = castlingRights;
    undo.prevEnPassant = enPassantSquare;
    undo.prevTurn = sideToMove;
    undo.captured = pieces[m.to];

    Piece moving = pieces[m.from];

    enPassantSquare = -1;

    if (m.flags & Flag::EN_PASSANT) {
        const int capSq = (sideToMove == WHITE ? m.to - 8 : m.to + 8);
        undo.captured = pieces[capSq];
        pieces[capSq] = EMPTY;
    }

    if (m.flags & Flag::CASTLE_KING) {
        if (sideToMove == WHITE) {
            pieces[5] = W_ROOK;
            pieces[7] = EMPTY;
        }
        else {
            pieces[61] = B_ROOK;
            pieces[63] = EMPTY;
        }
    }

    if (m.flags & Flag::CASTLE_QUEEN) {
        if (sideToMove == WHITE) {
            pieces[3] = W_ROOK;
            pieces[0] = EMPTY;
        }
        else {
            pieces[59] = B_ROOK;
            pieces[56] = EMPTY;
        }
    }

    pieces[m.to] = moving;
    pieces[m.from] = EMPTY;

    if (m.flags & Flag::PROMOTION) {
        pieces[m.to] = m.promotion;
    }

    if (m.flags & Flag::DOUBLE_PUSH) {
        enPassantSquare = (sideToMove == WHITE ? m.to - 8 : m.to + 8);
    }

    if (moving == W_KING) castlingRights &= ~(WK | WQ);
    if (moving == B_KING) castlingRights &= ~(BK | BQ);

    if (moving == W_ROOK) {
        if (m.from == 0) castlingRights &= ~WQ;
        if (m.from == 7) castlingRights &= ~WK;
    }
    else if (moving == B_ROOK) {
        if (m.from == 56) castlingRights &= ~BQ;
        if (m.from == 63) castlingRights &= ~BK;
    }

    if (undo.captured == W_ROOK) {
        if (m.to == 0) castlingRights &= ~WQ;
        if (m.to == 7) castlingRights &= ~WK;
    }
    else if (undo.captured == B_ROOK){
        if (m.to == 56) castlingRights &= ~BQ;
        if (m.to == 63) castlingRights &= ~BK;
    }

    flipTurn();

    return undo;
}

void Board::unmakeMove(const Move& m, const UndoState& undo) {
    sideToMove = undo.prevTurn;

    castlingRights = undo.prevCastlingRights;
    enPassantSquare = undo.prevEnPassant;

    Piece moving = pieces[m.to];

    if (m.flags & Flag::PROMOTION) {
        moving = (undo.prevTurn == WHITE ? W_PAWN : B_PAWN);
    }

    pieces[m.from] = moving;
    pieces[m.to] = undo.captured;

    if (m.flags & Flag::EN_PASSANT) {
        int capSq = (undo.prevTurn == WHITE ? m.to - 8 : m.to + 8);
        pieces[capSq] = undo.captured;
        pieces[m.to] = EMPTY;
    }

    if (m.flags & Flag::CASTLE_KING) {
        if (sideToMove == WHITE) {
            pieces[7] = W_ROOK;
            pieces[5] = EMPTY;
        } else {
            pieces[63] = B_ROOK;
            pieces[61] = EMPTY;
        }
    }

    if (m.flags & Flag::CASTLE_QUEEN) {
        if (sideToMove == WHITE) {
            pieces[0] = W_ROOK;
            pieces[3] = EMPTY;
        } else {
            pieces[56] = B_ROOK;
            pieces[59] = EMPTY;
        }
    }
}

}
