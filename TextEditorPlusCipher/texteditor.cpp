#include "texteditor.h"
#include "line.h"
#include "history.h"
#include "cipher_manager.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

void TextEditor::AddTextLine(const std::string& text)
{
    _history.SaveNode(_allLines, _cursor.lineInd, _cursor.symbolInd);

    _allLines.push_back(new TextLine(text));
    
    _cursor.lineInd = static_cast<int>(_allLines.size()) - 1;
    _cursor.symbolInd = text.length();
}

void TextEditor::AddCheckLine(const std::string& text, bool isChecked)
{
    _history.SaveNode(_allLines, _cursor.lineInd, _cursor.symbolInd);

    _allLines.push_back(new CheckLine(text, isChecked));

    _cursor.lineInd = static_cast<int>(_allLines.size()) - 1;
    _cursor.symbolInd = text.length() + 4;
}

void TextEditor::AddContactLine(const std::string& name, const std::string& email)
{
    _history.SaveNode(_allLines, _cursor.lineInd, _cursor.symbolInd);

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
        Line* line = Line::deserialize(newLine);
        if (line != nullptr)
        {
            _allLines.push_back(line);
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

    std::cout << "\n";
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

    _history.SaveNode(_allLines, _cursor.lineInd, _cursor.symbolInd);

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

    _history.SaveNode(_allLines, _cursor.lineInd, _cursor.symbolInd);

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

    _history.SaveNode(_allLines, _cursor.lineInd, _cursor.symbolInd);

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

void TextEditor::EncryptAllText(const std::string& cipherType, const std::string& key)
{
    if (_allLines.empty())
    {
        std::cout << "Nothing to encrypt. \n";
        return;
    }

    std::string text = "";

    for (Line* line : _allLines)
    {
        if (line != nullptr)
        {
            text += line -> serialize() + "\n";
        }
    }

    _encrypted = cipherManager.Encrypt(text, cipherType, key);
}

void TextEditor::DecryptAllText(const std::string& cipherType, const std::string& key)
{
    if (_encrypted.empty())
    {
        std::cout << "Nothing to decrypt. You need to encrypt first. \n";
        return;
    }

    _decrypted = cipherManager.Decrypt(_encrypted, cipherType, key);
}

void TextEditor::SaveEncrypted(const std::string& filename, const std::string& cipherType, const std::string& key)
{
    EncryptAllText(cipherType, key);

    std::ofstream file(filename);

    if(!file.is_open())
    {
        std::cout << "Cannot open the file. \n";
        return;
    }

    file << _encrypted;

    file.close();

    std::cout << "Encrypted and saved to file. \n";
}

void TextEditor::LoadEncrypted(const std::string& filename, const std::string& cipherType, const std::string& key)
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

    std::stringstream bufferToDecrypt;
    bufferToDecrypt << file.rdbuf();
    _encrypted = bufferToDecrypt.str();
    file.close();

    _decrypted = "";

    DecryptAllText(cipherType, key);

    if (_decrypted == "" && !_encrypted.empty())
    {
        std::cout << "Decryption failed. \n";
        return;
    }

    std::stringstream text(_decrypted);
    std::string row;

    while (std::getline(text, row))
    {
        if (row.empty())
        {
            continue;
        }

        Line* line = Line::deserialize(row);

        if (line != nullptr)
        {
            _allLines.push_back(line);
        }
    }
}

void TextEditor::ChangeStatus()
{
    if (_allLines.empty())
    {
        std::cout << "No text, cannot change status. \n";
        return;
    }

    _allLines[_cursor.lineInd] -> changeStatus();
}

TextEditor::~TextEditor()
{
    for (Line* line : _allLines)
    {
        delete line;
    }

    _allLines.clear();
}
