#include "board.hpp"
#include "constants.hpp"
#include <cstdlib>

namespace Chess {

Board::Board() {
    reset();
}

void Board::reset() {
    pieces.fill(EMPTY);

    for (int sq = 0; sq < 8; sq++) {
        pieces[sq + 8]  = W_PAWN;
        pieces[sq + 48] = B_PAWN;
    }

    pieces[0]  = W_ROOK;
    pieces[7]  = W_ROOK;
    pieces[56] = B_ROOK;
    pieces[63] = B_ROOK;

    pieces[1]  = W_KNIGHT;
    pieces[6]  = W_KNIGHT;
    pieces[57] = B_KNIGHT;
    pieces[62] = B_KNIGHT;

    pieces[2]  = W_BISHOP;
    pieces[5]  = W_BISHOP;
    pieces[58] = B_BISHOP;
    pieces[61] = B_BISHOP;

    pieces[3]  = W_QUEEN;
    pieces[59] = B_QUEEN;

    pieces[4]  = W_KING;
    pieces[60] = B_KING;

    whiteKingPos = 4;
    blackKingPos = 60;

    sideToMove      = WHITE;
    enPassantSquare = -1;
    castlingRights  = WK | WQ | BK | BQ;
}

void Board::printBoard() const {
    return;
}

UndoState Board::makeMove(const Move& move) {
    UndoState undo;

    undo.prevCastlingRights = castlingRights;
    undo.prevEnPassant      = enPassantSquare;
    undo.prevTurn           = sideToMove;
    undo.captured           = pieces[move.to];

    Piece moving = pieces[move.from];

    enPassantSquare = -1;

    if (move.flags & Flag::EN_PASSANT) {
        const int capSq = (sideToMove == WHITE ? move.to - 8 : move.to + 8);
        undo.captured   = pieces[capSq];
        pieces[capSq]   = EMPTY;
    }

    if (move.flags & Flag::CASTLE_KING) {
        if (sideToMove == WHITE) {
            pieces[5] = W_ROOK;
            pieces[7] = EMPTY;
        }
        else {
            pieces[61] = B_ROOK;
            pieces[63] = EMPTY;
        }
    }

    if (move.flags & Flag::CASTLE_QUEEN) {
        if (sideToMove == WHITE) {
            pieces[3] = W_ROOK;
            pieces[0] = EMPTY;
        }
        else {
            pieces[59] = B_ROOK;
            pieces[56] = EMPTY;
        }
    }

    pieces[move.to]   = moving;
    pieces[move.from] = EMPTY;

    if (move.flags & Flag::PROMOTION) {
        pieces[move.to] = move.promotion;
    }

    if (move.flags & Flag::DOUBLE_PUSH) {
        enPassantSquare = (sideToMove == WHITE ? move.to - 8 : move.to + 8);
    }

    if (moving == W_KING) {
        castlingRights &= ~(WK | WQ);
        whiteKingPos = move.to;
    }

    if (moving == B_KING) {
        castlingRights &= ~(BK | BQ);
        blackKingPos = move.to;
    }

    if (moving == W_ROOK) {
        if (move.from == 0)
            castlingRights &= ~WQ;
        if (move.from == 7)
            castlingRights &= ~WK;
    }
    else if (moving == B_ROOK) {
        if (move.from == 56)
            castlingRights &= ~BQ;
        if (move.from == 63)
            castlingRights &= ~BK;
    }

    if (undo.captured == W_ROOK) {
        if (move.to == 0)
            castlingRights &= ~WQ;
        if (move.to == 7)
            castlingRights &= ~WK;
    }
    else if (undo.captured == B_ROOK) {
        if (move.to == 56)
            castlingRights &= ~BQ;
        if (move.to == 63)
            castlingRights &= ~BK;
    }

    flipTurn();

    return undo;
}

void Board::unmakeMove(const Move& m, const UndoState& undo) {
    sideToMove = undo.prevTurn;

    castlingRights  = undo.prevCastlingRights;
    enPassantSquare = undo.prevEnPassant;

    Piece moving = pieces[m.to];

    if (m.flags & Flag::PROMOTION) {
        moving = (undo.prevTurn == WHITE ? W_PAWN : B_PAWN);
    }

    pieces[m.from] = moving;
    pieces[m.to]   = undo.captured;

    if (m.flags & Flag::EN_PASSANT) {
        int capSq     = (undo.prevTurn == WHITE ? m.to - 8 : m.to + 8);
        pieces[capSq] = undo.captured;
        pieces[m.to]  = EMPTY;
    }

    if (m.flags & Flag::CASTLE_KING) {
        if (sideToMove == WHITE) {
            pieces[7] = W_ROOK;
            pieces[5] = EMPTY;
        }
        else {
            pieces[63] = B_ROOK;
            pieces[61] = EMPTY;
        }
    }

    if (m.flags & Flag::CASTLE_QUEEN) {
        if (sideToMove == WHITE) {
            pieces[0] = W_ROOK;
            pieces[3] = EMPTY;
        }
        else {
            pieces[56] = B_ROOK;
            pieces[59] = EMPTY;
        }
    }
}

void Board::generateLegalMoves(std::vector<Move>& moves) {
    std::vector<Move> pseudoLegal;
    generatePseudoLegalMoves(pseudoLegal);

    const Color side = sideToMove;
    for (int i = 0; i < (int)pseudoLegal.size(); i++) {
        Move m = pseudoLegal[i];

        UndoState u = makeMove(m);
        if (!isInCheck(side))
            moves.emplace_back(m);
        unmakeMove(m, u);
    }
}

void Board::generatePseudoLegalMoves(std::vector<Move>& moves) const {
    for (int i = 0; i < 64; i++) {
        Piece piece = pieceAt(i);
        if (piece == EMPTY)
            continue;

        if (sideToMove == WHITE && !isWhite(piece))
            continue;
        if (sideToMove == BLACK && isWhite(piece))
            continue;

        switch (abs(piece)) {
        case W_PAWN:
            genPawnMoves(moves, i);
            break;
        case W_KNIGHT:
            genKnightMoves(moves, i);
            break;
        case W_BISHOP:
            genBishopMoves(moves, i);
            break;
        case W_ROOK:
            genRookMoves(moves, i);
            break;
        case W_QUEEN:
            genQueenMoves(moves, i);
            break;
        case W_KING:
            genKingMoves(moves, i);
            break;
        }
    }
}

bool Board::isInCheck(Color side) const {
    if (side == WHITE)
        return isSquareAttacked(whiteKingPos, BLACK);
    else
        return isSquareAttacked(blackKingPos, WHITE);
}

bool Board::isSquareAttacked(const int sq, Color attacking) const {
    const int sqFile = sq % 8;

    // pawns
    if (attacking == WHITE) {
        static const int offsets[2] = {-7, -9};
        for (const int offset : offsets) {
            const int target = sq + offset;
            if (target >= 0 && target < 64) {
                const int targetFile = target % 8;

                if (abs(targetFile - sqFile) == 1 && pieceAt(target) == W_PAWN)
                    return true;
            }
        }
    }
    else { // black
        static const int offsets[2] = {7, 9};
        for (const int offset : offsets) {
            const int target = sq + offset;
            if (target >= 0 && target < 64) {
                const int targetFile = target % 8;

                if (abs(targetFile - sqFile) == 1 && pieceAt(target) == B_PAWN)
                    return true;
            }
        }
    }

    // knights
    static const int knightOffsets[8] = {17, 15, 10, 6, -6, -10, -15, -17};

    for (const int offset : knightOffsets) {
        const int target = sq + offset;
        if (target < 0 || target >= 64) {
            continue;
        }

        const int targetFile = target % 8;
        if (abs(targetFile - sqFile) <= 2) {
            if (attacking == WHITE && pieceAt(target) == W_KNIGHT)
                return true;
            if (attacking == BLACK && pieceAt(target) == B_KNIGHT)
                return true;
        }
    }

    // bishops / queens (disgonal)
    static const int bishopOffsets[4] = {9, 7, -7, -9};
    for (const int offset : bishopOffsets) {
        int currentSquare = sq + offset;
        while (currentSquare >= 0 && currentSquare < 64) {
            int currentFile = currentSquare % 8;
            int prevFile    = (currentSquare - offset) % 8;

            if (abs(currentFile - prevFile) != 1)
                break;

            Piece currentPiece = pieceAt(currentSquare);
            if (currentPiece != EMPTY) {
                if (attacking == WHITE && (currentPiece == W_BISHOP || currentPiece == W_QUEEN))
                    return true;
                else if (attacking == BLACK && (currentPiece == B_BISHOP || currentPiece == B_QUEEN))
                    return true;
                else
                    break; // hit another piece
            }
            currentSquare += offset;
        }
    }

    // rook / queen (orthogonal)
    static const int rookOffsets[4] = {8, 1, -1, -8};
    for (const int offset : rookOffsets) {
        int currentSquare = sq + offset;
        while (currentSquare >= 0 && currentSquare < 64) {
            int currentFile = currentSquare % 8;

            if (offset == 1 && currentFile == 0) // moving left, break if wrap from 0 to 7
                break;
            if (offset == -1 && currentFile == 7) // same for right, 7 to 0
                break;

            Piece currentPiece = pieceAt(currentSquare);
            if (currentPiece != EMPTY) {
                if (attacking == WHITE && (currentPiece == W_ROOK || currentPiece == W_QUEEN))
                    return true;
                else if (attacking == BLACK && (currentPiece == B_ROOK || currentPiece == B_QUEEN))
                    return true;
                else
                    break;
            }
            currentSquare += offset;
        }
    }

    // kings
    static const int kingOffsets[8] = {9, 8, 7, 1, -1, -7, -8, -9};
    for (const int offset : kingOffsets) {
        int targetSquare = sq + offset;
        if (targetSquare < 0 || targetSquare >= 64)
            continue;

        int targetFile = targetSquare % 8;
        if (abs(targetFile - sqFile) <= 1) {
            Piece currentPiece = pieceAt(targetSquare);
            if (attacking == WHITE && currentPiece == W_KING)
                return true;
            else if (attacking == BLACK && currentPiece == B_KING)
                return true;
        }
    }

    return false;
}

void Board::genPawnMoves(std::vector<Move>& moves, const int sq) const {
    const Piece p        = pieceAt(sq);
    const Color friendly = sideToMove;
    if (p != W_PAWN && p != B_PAWN)
        return;

    const int direction = (p == W_PAWN) ? 8 : -8;
    const int startRank = (p == W_PAWN) ? 1 : 6;

    const int rank       = sq / 8;
    const int forwardOne = sq + direction;

    if (forwardOne >= 0 && forwardOne < 64 && pieceAt(forwardOne) == EMPTY) {
        int toRank = forwardOne / 8;
        if ((p == W_PAWN && toRank == 7) || (p == B_PAWN && toRank == 0)) {
            moves.emplace_back(sq, forwardOne, p == W_PAWN ? W_KNIGHT : B_KNIGHT, PROMOTION | PROMOTION_KNIGHT);
            moves.emplace_back(sq, forwardOne, p == W_PAWN ? W_BISHOP : B_BISHOP, PROMOTION | PROMOTION_BISHOP);
            moves.emplace_back(sq, forwardOne, p == W_PAWN ? W_ROOK : B_ROOK, PROMOTION | PROMOTION_ROOK);
            moves.emplace_back(sq, forwardOne, p == W_PAWN ? W_QUEEN : B_QUEEN, PROMOTION | PROMOTION_QUEEN);
        }
        else {
            moves.emplace_back(sq, forwardOne, EMPTY);
        }

        if (rank == startRank) {
            int forwardTwo = forwardOne + direction;
            if (forwardTwo >= 0 && forwardTwo < 64 && pieceAt(forwardTwo) == EMPTY) {
                moves.emplace_back(sq, forwardTwo, EMPTY, DOUBLE_PUSH);
            }
        }
    }

    const int captureOffets[2] = {direction + 1, direction - 1};
    for (const int offset : captureOffets) {
        const int capSq = sq + offset;
        if (capSq < 0 || capSq >= 64)
            continue;

        const int file    = sq % 8;
        const int capFile = capSq % 8;
        if (capFile != file - 1 && capFile != file + 1)
            continue;

        const Piece target = pieceAt(capSq);
        if (target != EMPTY && isWhite(target) != (friendly == WHITE)) {
            const int capRank = capSq / 8;
            if ((p == W_PAWN && capRank == 7) || (p == B_PAWN && capRank == 0)) {
                moves.emplace_back(sq, capSq, p == W_PAWN ? W_KNIGHT : B_KNIGHT, CAPTURE | PROMOTION | PROMOTION_KNIGHT);
                moves.emplace_back(sq, capSq, p == W_PAWN ? W_BISHOP : B_BISHOP, CAPTURE | PROMOTION | PROMOTION_BISHOP);
                moves.emplace_back(sq, capSq, p == W_PAWN ? W_ROOK : B_ROOK, CAPTURE | PROMOTION | PROMOTION_ROOK);
                moves.emplace_back(sq, capSq, p == W_PAWN ? W_QUEEN : B_QUEEN, CAPTURE | PROMOTION | PROMOTION_QUEEN);
            }
            else {
                moves.emplace_back(sq, capSq, EMPTY, CAPTURE);
            }
        }
        else if (capSq == enPassantSquare) {
            moves.emplace_back(sq, capSq, EMPTY, EN_PASSANT);
        }
    }
}

void Board::genKnightMoves(std::vector<Move>& moves, const int sq) const {
    static const int offsets[8] = {17, 15, 10, 6, -6, -10, -15, -17};

    Color friendly = sideToMove;

    for (const int off : offsets) {
        int to = sq + off;
        if (to < 0 || to >= 64)
            continue;

        int fromFile = sq % 8;
        int toFile   = to % 8;

        if (abs(toFile - fromFile) > 2)
            continue;

        Piece target = pieceAt(to);
        if (target == EMPTY) {
            moves.emplace_back(sq, to, EMPTY);
        }
        else if (isWhite(target) != (friendly == WHITE)) {
            moves.emplace_back(sq, to, EMPTY, CAPTURE);
        }
    }
}

void Board::genBishopMoves(std::vector<Move>& moves, const int sq) const {
    static const int bishopDirs[4] = {9, 7, -7, -9};
    genSlidingMoves(moves, sq, bishopDirs, 4);
}

void Board::genRookMoves(std::vector<Move>& moves, const int sq) const {
    static const int rookDirs[4] = {8, 1, -1, -8};
    genSlidingMoves(moves, sq, rookDirs, 4);
}

void Board::genQueenMoves(std::vector<Move>& moves, const int sq) const {
    static const int queenDirs[8] = {9, 8, 7, 1, -1, -7, -8, -9};
    genSlidingMoves(moves, sq, queenDirs, 8);
}

void Board::genKingMoves(std::vector<Move>& moves, const int sq) const {
    const Piece p        = pieceAt(sq);
    const int fromFile   = sq % 8;
    const Color friendly = sideToMove;

    static const int kingDirs[8] = {9, 8, 7, 1, -1, -7, -8, -9};
    for (const int offset : kingDirs) {
        const int target = sq + offset;
        if (target < 0 || target >= 64)
            continue;

        const int toFile = target % 8;
        if (abs(fromFile - toFile) > 1)
            continue;

        const Piece targetPiece = pieceAt(target);
        if (targetPiece == EMPTY) {
            moves.emplace_back(sq, target, EMPTY);
        }
        else if (isWhite(targetPiece) != (friendly == WHITE)) {
            moves.emplace_back(sq, target, EMPTY, CAPTURE);
        }
    }

    // castling

    // white kingside
    if (canWhiteCastleKingside() && isWhite(p) && !isSquareAttacked(sq, BLACK)) {

        bool empty = (pieceAt(5) == EMPTY && pieceAt(6) == EMPTY);
        bool safe  = (!isSquareAttacked(5, BLACK) && !isSquareAttacked(6, BLACK));

        if (empty && safe)
            moves.emplace_back(sq, 6, EMPTY, CASTLE_KING);
    }

    // white queenside
    if (canWhiteCastleQueenside() && isWhite(p) && !isSquareAttacked(sq, BLACK)) {

        bool empty = (pieceAt(3) == EMPTY && pieceAt(2) == EMPTY && pieceAt(1) == EMPTY);
        bool safe  = (!isSquareAttacked(3, BLACK) && !isSquareAttacked(2, BLACK));

        if (empty && safe)
            moves.emplace_back(sq, 2, EMPTY, CASTLE_QUEEN);
    }

    // black kingside
    if (canBlackCastleKingside() && !isWhite(p) && !isSquareAttacked(sq, WHITE)) {

        bool empty = (pieceAt(61) == EMPTY && pieceAt(62) == EMPTY);
        bool safe  = (!isSquareAttacked(61, WHITE) && !isSquareAttacked(62, WHITE));

        if (empty && safe)
            moves.emplace_back(sq, 62, EMPTY, CASTLE_KING);
    }

    // black queenside
    if (canBlackCastleQueenside() && !isWhite(p) && !isSquareAttacked(sq, WHITE)) {

        bool empty = (pieceAt(59) == EMPTY && pieceAt(58) == EMPTY && pieceAt(57) == EMPTY);
        bool safe  = (!isSquareAttacked(59, WHITE) && !isSquareAttacked(58, WHITE));

        if (empty && safe)
            moves.emplace_back(sq, 58, EMPTY, CASTLE_QUEEN);
    }
}

void Board::genSlidingMoves(std::vector<Move>& moves, const int sq, const int* directions, const int dirCount) const {
    Color friendly = sideToMove;

    for (int i = 0; i < dirCount; i++) {
        const int dir = directions[i];
        int prev      = sq;

        while (true) {
            int fromFile = prev % 8;

            if ((dir == 1 || dir == 9 || dir == -7) && fromFile == 7)
                break;

            if ((dir == -1 || dir == -9 || dir == 7) && fromFile == 0)
                break;

            int to = prev + dir;
            if (to < 0 || to >= 64)
                break;

            Piece target = pieceAt(to);

            if (target == EMPTY) {
                moves.emplace_back(sq, to, EMPTY);
            }
            else {
                if (isWhite(target) != (friendly == WHITE)) {
                    moves.emplace_back(sq, to, EMPTY, CAPTURE);
                }
                break;
            }

            prev = to;
        }
    }
}

} // namespace Chess
