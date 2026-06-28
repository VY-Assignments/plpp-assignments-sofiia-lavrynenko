#include "texteditor.h"
#include "line.h"
#include "history.h"

#include <iostream>
#include <string>
#include <fstream>

void TextEditor::AddTextLine(const std::string& text)
{
    _allLines.push_back(new TextLine(text));
    
    _cursor.lineInd = static_cast<int>(_allLines.size()) - 1;
    _cursor.symbolInd = text.length();
}

void TextEditor::AddCheckLine(const std::string& text, bool isChecked)
{
    _allLines.push_back(new CheckLine(text, isChecked));

    _cursor.lineInd = static_cast<int>(_allLines.size()) - 1;
    _cursor.symbolInd = text.length() + 4;
}

void TextEditor::AddContactLine(const std::string& name, const std::string& email)
{
    _allLines.push_back(new ContactLine(name, email));

    _cursor.lineInd = static_cast<int>(_allLines.size()) - 1;
    _cursor.symbolInd = name.length() + email.length() + 16;
}

void TextEditor::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Cannot open the file. \n";
        return;
    }

    for (Line* line : _allLines)
    {
        file << line -> serialize() << "\n";
    }

    file.close();

    std::cout << "Saved to file. \n";
}

void TextEditor::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Cannot open the file. \n";
        return;
    }

    for (Line* line : _allLines)
    {
        delete line;
    }

    _allLines.clear();

    std::string newLine;

    while (std::getline(file, newLine))
    {
        Line* line;

        if (newLine[0] == 'T')
        {
            AddTextLine(newLine.erase(0, 4));
        }
        else if (newLine[0] == 'C')
        {
            bool check;

            if (newLine[4] == '1')
            {
                check = true;
            }
            else
            {
                check = false;
            }

            AddCheckLine(newLine.erase(0, 8), check);
        }
        else
        {
            newLine = newLine.erase(0, 4);

            size_t position = newLine.find('|');

            std::string name = newLine.substr(0, position);

            std::string email = newLine.erase(0, name.length() + 3);

            AddContactLine(name, email);
        }
    }

    file.close();

    std::cout << "Loaded from file. \n";
}

void TextEditor::PrintToConsole() const
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to print. \n";
        return;
    }

    for (size_t i = 0; i < _allLines.size(); i++)
    {
        _allLines[i] -> print();
    }
}

TextEditor::~TextEditor()
{
    for (Line* line : _allLines)
    {
        delete line;
    }

    _allLines.clear();
}

void TextEditor::MoveCursor(int newLineInd, int newSymbolInd)
{
    if (_allLines.empty())
    {
        _cursor.lineInd = 0;
        _cursor.symbolInd = 0;
        return;
    }

    if (newLineInd < 0)
    {
        newLineInd = 0;
    }
    else if (newLineInd >= static_cast<int>(_allLines.size()))
    {
        std::cout << "New line index cannot be more than text size. \n";
        return;
    }
    
    if (newSymbolInd < 0)
    {
        newSymbolInd = 0;
    }
    else if (newSymbolInd > _allLines[newLineInd] -> getLength())
    {
        std::cout << "New syymbol index cannot be more than text size. \n";
        return;
    }

    _cursor.lineInd = newLineInd;
    _cursor.symbolInd = newSymbolInd;
}

void TextEditor::InsertAt(std::string toInsert)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to insert into. \n";
        return;
    }

    _allLines[_cursor.lineInd] -> insert(_cursor.symbolInd, toInsert);

    _cursor.symbolInd += toInsert.length();
}

void TextEditor::InsertWithReplace(std::string toInsert)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to insert into. \n";
        return;
    }

    _allLines[_cursor.lineInd] -> insertWithReplace(_cursor.symbolInd, toInsert);

    int max = _allLines[_cursor.lineInd] -> getLength();
    if (_cursor.symbolInd > max)
    {
        _cursor.symbolInd = max;
    }
}

void TextEditor::DeleteAt(int symbolsToDelete)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to delete from. \n";
        return;
    }

    _allLines[_cursor.lineInd] -> deleteAt(_cursor.symbolInd, symbolsToDelete);
}

void TextEditor::Copy(int symbolsToCopy)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to copy from. \n";
        return;
    }

    _allLines[_cursor.lineInd] -> copy(_cursor.symbolInd, symbolsToCopy, _clipboard);
}

void TextEditor::Paste()
{
    if (_clipboard.empty())
    {
        std::cout << "Clipboard is empty, nothing to paste. \n";
        return;
    }

    InsertAt(_clipboard);
}

void TextEditor::Cut(int symbolsToCut)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to cut from. \n";
        return;
    }

    Copy(symbolsToCut);

    DeleteAt(symbolsToCut);
}

void TextEditor::SearchByWord(std::string searchWord)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to search from. \n";
        return;
    }

    if (searchWord.empty())
    {
        std::cout << "You need to give a word. \n";
        return;
    }

    bool found = false;

    std::cout << "Found at indexes: \n";

    for (int i = 0; i < _allLines.size(); i++)
    {
        std::vector<int> positions = _allLines[i] -> search(searchWord);

        for (int symbol : positions)
        {
            std::cout << "line: " << i << " | symbol: " << symbol << "\n";

            if (!found)
            {
                _cursor.lineInd = i;
                _cursor.symbolInd = symbol;

                found = true;
            }
        }
    }

    if (!found)
    {
        std::cout << "Nothing found. \n";
    }
}

void TextEditor::Undo()
{
    _history.Undo(_allLines, _cursor.lineInd, _cursor.symbolInd);
}

void TextEditor::Redo()
{
    _history.Redo(_allLines, _cursor.lineInd, _cursor.symbolInd);
}