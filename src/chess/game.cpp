#include "game.hpp"
#include "board.hpp"

namespace Chess
{

GameManager::GameManager() :
    hasSelection(false),
    selectionRow(0),
    selectionCol(0),
    undoIndex(0)
{
    board.reset();
}

void GameManager::selectSquare(const int cursorRow, const int cursorCol) {

    if (hasSelection && cursorRow == selectionRow && cursorCol == selectionCol){
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
    // TODO
    return;
}

}
