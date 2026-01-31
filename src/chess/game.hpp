#pragma once

#include <string>
#include <vector>

#include "constants.hpp"
#include "move.hpp"
#include "board.hpp"

namespace Chess 
{

class GameManager
{

public:
    GameManager();

    void selectSquare(const int cursorRow, const int cursorCol);
    void cancelSelection();

    [[nodiscard]] inline bool isSelected(int r, int c) const {
        return hasSelection && selectionRow == r && selectionCol == c;
    }

    [[nodiscard]] bool isLegalSq(const int row, const int col) const;

    [[nodiscard]] inline Color getTurn() const { return board.getTurn(); }
    [[nodiscard]] inline Piece pieceAt(const int row, const int col) const { return board.pieceAt(row, col); };
    [[nodiscard]] inline bool isWhite(Piece p) const { return p > 0; }

    bool attemptMove(const int row, const int col);

    inline int getSelectedRow() const { return selectionRow; }
    inline int getSelectedCol() const { return selectionCol; }

    void undo();
    void redo();
    void goToStartPos();
    void goToCurrPos();

    std::string getUciPos() const;
    Move parseUciMove(const std::string& uciMove) const;
    void applyEngineMove(const std::string& uciMove);

    [[nodiscard]] inline bool isEngineTurn() const { return board.getTurn() == BLACK; } // TODO: allow user to choose their side

private:
    Board board;

    bool hasSelection;
    int selectionRow;
    int selectionCol;

    std::vector<Move> moveHistory;
    std::vector<UndoState> undoHistory;
    int undoIndex = 0;

    std::vector<Move> legalMoves;

    void makeMove(const Move& m);
    void updateLegalMoves();
};

}
