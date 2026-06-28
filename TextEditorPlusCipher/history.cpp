#include <vector>
#include <stack>
#include <iostream>
#include "line.h"
#include "history.h"

History::History() {}

std::vector<Line*> History::Clone(const std::vector<Line*>& orig)
{
    std::vector<Line*> copy;

    for (Line* line : orig)
    {
        if (line != nullptr)
        {
            copy.push_back(line -> clone());
        }
    }

    return copy;
}

void History::ClearStack(std::stack<HistoryNode>& stack)
{
    while (!stack.empty())
    {
        HistoryNode node = stack.top();

        for (Line* line : node.lines)
        {
            delete line;
        }

        stack.pop();
    }
}

void History::ClearRedo()
{
    ClearStack(_redo);
}

void History::Undo(std::vector<Line*>& lines, int& cursorLine, int& cursorSymbol)
{
    if (_undo.empty())
    {
        std::cout << "Nothing to undo. \n";
        return;
    }

    HistoryNode current;

    current.lines = Clone(lines);
    current.cursorLine = cursorLine;
    current.cursorSymbol = cursorSymbol;

    _redo.push(current);

    for (Line* line : lines)
    {
        delete line;
    }

    HistoryNode previous = _undo.top();
    _undo.pop();

    lines = previous.lines;
    cursorLine = previous.cursorLine;
    cursorSymbol = previous.cursorSymbol;
}

void History::Redo(std::vector<Line*>& lines, int& cursorLine, int& cursorSymbol)
{
    if (_redo.empty())
    {
        std::cout << "Nothing to redo. \n";
        return;
    }

    HistoryNode current;

    current.lines = Clone(lines);
    current.cursorLine = cursorLine;
    current.cursorSymbol = cursorSymbol;

    _undo.push(current);

    for (Line* line : lines)
    {
        delete line;
    }

    HistoryNode next = _redo.top();
    _redo.pop();

    lines = next.lines;
    cursorLine = next.cursorLine;
    cursorSymbol = next.cursorSymbol;
}

void History::SaveNode(const std::vector<Line*>& lines, int cursorLine, int cursorSymbol)
{
    HistoryNode newNode;

    newNode.lines = Clone(lines);
    newNode.cursorLine = cursorLine;
    newNode.cursorSymbol = cursorSymbol;

    _undo.push(newNode);

    ClearRedo();
}

History::~History()
{
    ClearStack(_undo);
    ClearStack(_undo);
}