#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <stack>
#include "line.h"

struct HistoryNode
{
    std::vector<Line*> lines;
    int cursorLine;
    int cursorSymbol;
};

class History
{
    private:
        std::stack<HistoryNode> _undo;
        std::stack<HistoryNode> _redo;

        void ClearStack(std::stack<HistoryNode>& stack);
        std::vector<Line*> Clone(const std::vector<Line*>& orig);

    public:
        History();

        void SaveNode(const std::vector<Line*>& lines, int cursorLine, int cursorSymbol);
        void Undo(std::vector<Line*>& lines, int& cursorLine, int& cursorSymbol);
        void Redo(std::vector<Line*>& lines, int& cursorLine, int& cursorSymbol);
        void ClearRedo();

        ~History();
};

#endif