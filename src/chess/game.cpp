#include "game.hpp"
#include "board.hpp"
#include "constants.hpp"

namespace Chess {

GameManager::GameManager() : hasSelection(false), selectionRow(0), selectionCol(0), undoIndex(0) {
    board.reset();
}

void GameManager::selectSquare(const int cursorRow, const int cursorCol) {

    if (hasSelection && cursorRow == selectionRow && cursorCol == selectionCol) {
        hasSelection = false;
        return;
    }

    if (hasSelection && isLegalSq(cursorRow, cursorCol)) {
        attemptMove(cursorRow, cursorCol);
        return;
    }

    const Piece p = pieceAt(cursorRow, cursorCol);

    if (p != EMPTY && board.isWhite(p) == (board.getTurn() == WHITE)) {
        hasSelection = true;
        selectionRow = cursorRow;
        selectionCol = cursorCol;

        updateLegalMoves();
        return;
    }

    cancelSelection();
}

void GameManager::cancelSelection() {
    hasSelection = false;
    legalMoves.clear();
}

bool GameManager::isLegalSq(const int row, const int col) const {
    for (const Move& m : legalMoves) {
        if (m.toRow() == row && m.toCol() == col)
            return true;
    }
    return false;
}

bool GameManager::attemptMove(const int row, const int col) {
    for (const Move& m : legalMoves) {
        if (m.toRow() == row && m.toCol() == col) {
            makeMove(m);
            return true;
        }
    }
    return false;
}
void GameManager::makeMove(const Move& m) {
    if (undoIndex != (int)moveHistory.size())
        return;

    UndoState u = board.makeMove(m);

    moveHistory.push_back(m);
    undoHistory.push_back(u);

    hasSelection = false;
    legalMoves.clear();
    undoIndex++;
}

void GameManager::undo() {
    if (undoIndex == 0)
        return;

    undoIndex--;
    board.unmakeMove(moveHistory[undoIndex], undoHistory[undoIndex]);
}

void GameManager::redo() {
    if (undoIndex == (int)moveHistory.size())
        return;

    board.makeMove(moveHistory[undoIndex]);
    undoIndex++;
}

void GameManager::goToStartPos() {
    while (undoIndex > 0)
        undo();
}

void GameManager::goToCurrPos() {
    while (undoIndex < (int)moveHistory.size())
        redo();
}

void GameManager::updateLegalMoves() {
    legalMoves.clear();

    std::vector<Move> allMoves;
    board.generateLegalMoves(allMoves);

    const int fromSq = selectionRow * 8 + selectionCol;

    for (const Move& m : allMoves) {
        if (m.from == fromSq)
            legalMoves.emplace_back(m);
    }
}

std::string GameManager::getUciPos() const {
    std::string cmd = "position startpos";

    if (!moveHistory.empty()) {
        cmd += " moves";
        for (const Move& m : moveHistory) {
            cmd += " ";
            cmd += m.toUci();
        }
    }

    return cmd;
}

Move GameManager::parseUciMove(const std::string& uciMove) const {
    const int fromCol = uciMove[0] - 'a';
    const int fromRow = 7 - (uciMove[1] - '1');

    const int toCol = uciMove[2] - 'a';
    const int toRow = 7 - (uciMove[3] - '1');

    const int from = fromRow * 8 + fromCol;
    const int to   = toRow * 8 + toCol;

    const Piece moving = board.pieceAt(from);
    const bool isWhite = board.isWhite(moving);

    Piece promoPiece = EMPTY;
    int flags        = NONE;

    if (uciMove.size() == 5) { // promotion
        flags |= PROMOTION;

        switch (uciMove[4]) {
        case 'n':
            promoPiece = isWhite ? W_KNIGHT : B_KNIGHT;
            flags |= PROMOTION_KNIGHT;
            break;

        case 'b':
            promoPiece = isWhite ? W_BISHOP : B_BISHOP;
            flags |= PROMOTION_BISHOP;
            break;

        case 'r':
            promoPiece = isWhite ? W_ROOK : B_ROOK;
            flags |= PROMOTION_ROOK;
            break;

        case 'q':
            promoPiece = isWhite ? W_QUEEN : B_QUEEN;
            flags |= PROMOTION_QUEEN;
            break;
        }
    }

    if (to == board.getEnPassantSq() && board.pieceAt(to) == EMPTY) {
        flags |= EN_PASSANT;
        flags |= CAPTURE;
    }
    else if (board.pieceAt(to) != EMPTY) {
        flags |= CAPTURE;
    }

    if (abs(to - from) == 16 && (moving == W_PAWN || moving == B_PAWN))
        flags |= DOUBLE_PUSH;

    if (moving == W_KING || moving == B_KING) {
        if (fromCol == 4 && toCol == 6)
            flags |= CASTLE_KING;
        if (fromCol == 4 && toCol == 2)
            flags |= CASTLE_QUEEN;
    }

    return Move{from, to, promoPiece, flags};
}

void GameManager::applyEngineMove(const std::string& uciMove) {
    if (undoIndex != (int)moveHistory.size())
        return;

    Move m      = parseUciMove(uciMove);
    UndoState u = board.makeMove(m);

    moveHistory.push_back(m);
    undoHistory.push_back(u);
    undoIndex++;

    hasSelection = false;

    legalMoves.clear();
    updateLegalMoves();
}

} // namespace Chess
