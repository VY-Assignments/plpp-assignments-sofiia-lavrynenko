#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "line.h"
#include <string>
#include <vector>
#include "history.h"
#include "cipher_manager.h"

struct Cursor
{
    int lineInd = 0;
    int symbolInd = 0;
};

class TextEditor 
{
    private:
        std::vector<Line*> _allLines;
        std::string _clipboard;
        std::string _encrypted;
        std::string _decrypted;
        Cursor _cursor;
        History _history;
        CipherManager cipherManager;

    public:
        TextEditor() = default;

        void AddTextLine(const std::string& text);
        void AddCheckLine(const std::string& text, bool isChecked);
        void AddContactLine(const std::string& name, const std::string& email);

        void ChangeStatus();

        void SaveToFile(const std::string& filename) const;
        void LoadFromFile(const std::string& filename);

        void PrintToConsole() const;
        void MoveCursor(int newLineInd, int newSymbolInd);

        void InsertAt(std::string toInsert);
        void InsertWithReplace(std::string toInsert);
        void DeleteAt(int symbolsToDelete);
        void Copy(int symbolsToCopy);
        void Paste();
        void Cut(int symbolsToCut);
        void SearchByWord(std::string searchWord);

        void Undo();
        void Redo();

        void EncryptAllText(const std::string& cipherType, const std::string& key);
        void DecryptAllText(const std::string& cipherType, const std::string& key);

        void SaveEncrypted(const std::string& filename, const std::string& cipherType, const std::string& key);
        void LoadEncrypted(const std::string& filename, const std::string& cipherType, const std::string& key);

        ~TextEditor();
};

#endif