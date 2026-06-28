#include "line.h"
#include <iostream>
#include <string>

TextLine::TextLine(const std::string& text)
{
    _text = text;
}

void TextLine::print() const
{
    std::cout << _text << "\n";
}

std::string TextLine::serialize() const
{
    return "T | " + _text;
}

int TextLine::getLength() 
{
    return _text.length();
}

void TextLine::insert(int index, std::string toInsert)
{
    if (index > _text.length())
    {
        index = _text.length();
    }

    std::string before = _text.substr(0, index);
    std::string after = _text.substr(index);

    _text = before + toInsert + after;
}

void TextLine::insertWithReplace(int index, std::string toInsert) 
{
    if (index > _text.length())
    {
        index = _text.length();
    }

    std::string before = _text.substr(0, index);

    std::string after = "";

    if (index + toInsert.length() < _text.length())
    {
        after = _text.substr(index + toInsert.length());
    }

    _text = before + toInsert + after;
}

void TextLine::deleteAt(int index, int symbolsToDelete)
{
    if (index > _text.length())
    {
        std::cout << "Nothing to delete after " << index << "\n";
        return;
    }

    std::string before = _text.substr(0, index);

    std::string after = _text.substr(0, index);

    if (index + symbolsToDelete < _text.length())
    {
        after = _text.substr(index + symbolsToDelete);
    }

    _text = before + after;
}

void TextLine::copy(int index, int symbolsToCopy, std::string& clipboard)
{
    if (index > _text.length())
    {
        index = _text.length();
    }

    if (index + symbolsToCopy > _text.length())
    {
        symbolsToCopy = _text.length() - index;
    }

    clipboard = _text.substr(index, symbolsToCopy);
}

std::vector<int> TextLine::search(std::string search)
{
    std::vector<int> results;

    int position = _text.find(search, 0);

    while (position != std::string::npos)
    {
        results.push_back(position);

        position = _text.find(search, position + search.length());
    }

    return results;
}

Line* TextLine::clone() const
{
    return new TextLine(*this);
}

CheckLine::CheckLine(const std::string& text, bool isChecked)
{
    _text = text;
    _isChecked = isChecked;
}

void CheckLine::print() const
{
    if (_isChecked)
    {
        std::cout << "[x] " << _text << "\n";
    }
    else
    {
        std::cout << "[ ] " << _text << "\n";
    }
}

std::string CheckLine::serialize() const
{
    std::string status;

    if (_isChecked)
    {
        status = "1";
    }
    else
    {
        status = "0";
    }

    return "C | " + status + " | " + _text;
}

int CheckLine::getLength() 
{
    return _text.length() + 4;
}

void CheckLine::insert(int index, std::string toInsert)
{
    if (index <= 3)
    {
        index = 4; // cannot insert in '[x] '
    }

    int realIndex = index - 4;

    if (realIndex > _text.length())
    {
        realIndex = _text.length();
    }

    std::string before = _text.substr(0, realIndex);
    std::string after = _text.substr(realIndex);

    _text = before + toInsert + after;
}

void CheckLine::insertWithReplace(int index, std::string toInsert)
{
    if (index <= 3)
    {
        index = 4; // cannot insert in '[x] '
    }

    int realIndex = index - 4;

    if (realIndex > _text.length())
    {
        realIndex = _text.length();
    }

    std::string before = _text.substr(0, realIndex);

    std::string after = "";

    if (realIndex + toInsert.length() < _text.length())
    {
        after = _text.substr(realIndex + toInsert.length());
    }

    _text = before + toInsert + after;
}

void CheckLine::deleteAt(int index, int symbolsToDelete)
{
    if (index <= 3)
    {
        index = 4; // cannot delete '[x] '
    }

    int realIndex = index - 4;

    std::string before = _text.substr(0, realIndex);

    std::string after = "";

    if (realIndex + symbolsToDelete < _text.length())
    {
        after = _text.substr(realIndex + symbolsToDelete);
    }

    _text = before + after;
}

void CheckLine::copy(int index, int symbolsToCopy, std::string& clipboard)
{
    if (index <= 3)
    {
        index = 4; // cannot copy '[x] '
    }

    int realIndex = index - 4;

    if (realIndex > _text.length())
    {
        realIndex = _text.length();
    }

    if (realIndex + symbolsToCopy > _text.length())
    {
        symbolsToCopy = _text.length() - realIndex;
    }

    clipboard = _text.substr(realIndex, symbolsToCopy);
}

std::vector<int> CheckLine::search(std::string search)
{
    std::vector<int> results;

    int position = _text.find(search, 0);

    while (position != std::string::npos)
    {
        results.push_back(position + 4);

        position = _text.find(search, position + search.length());
    }

    return results;
}

Line* CheckLine::clone() const
{
    return new CheckLine(*this);
}

ContactLine::ContactLine(const std::string& name, const std::string& email)
{
    _name = name;
    _email = email;
}

void ContactLine::print() const
{
    std::cout << "Name: " << _name << " | Email: " << _email << "\n";
}

std::string ContactLine::serialize() const
{
    return "I | " + _name + " | " + _email;
}

int ContactLine::getLength() 
{
    return _name.length() + _email.length() + 16;
}

void ContactLine::insert(int index, std::string toInsert)
{
    int startName = 6;
    int endName = startName + _name.length();

    if (index <= endName)
    {
        if (index < startName)
        {
            index = startName;
        }

        int realIndex = index - startName;

        std::string before = _name.substr(0, realIndex);
        std::string after = _name.substr(realIndex);

        _name = before + toInsert + after;
    }
    else
    {
        int startEmail = _name.length() + 15;

        if (index < startEmail)
        {
            index = startEmail;
        }

        int realIndex = index - startEmail;

        if (realIndex > _email.length())
        {
            realIndex = _email.length();
        }

        std::string before = _email.substr(0, realIndex);
        std::string after = _email.substr(realIndex);
        _email = before + toInsert + after;
    }
}

void ContactLine::insertWithReplace(int index, std::string toInsert)
{
    int startName = 6;
    int endName = startName + _name.length();

    if (index <= endName)
    {
        if (index < startName)
        {
            index = startName;
        }

        int realIndex = index - startName;

        std::string before = _name.substr(0, realIndex);

        std::string after = "";

        if (realIndex + toInsert.length() < _name.length())
        {
            after = _name.substr(realIndex + toInsert.length());
        }

        _name = before + toInsert + after;
    }
    else
    {
        int startEmail = _name.length() + 15;

        if (index < startEmail)
        {
            index = startEmail;
        }

        int realIndex = index - startEmail;

        if (realIndex > _email.length())
        {
            realIndex = _email.length();
        }

        std::string before = _email.substr(0, realIndex);
        
        std::string after = "";

        if (realIndex + toInsert.length() < _email.length())
        {
            after = _email.substr(realIndex + toInsert.length());
        }

        _email = before + toInsert + after;
    }
}

void ContactLine::deleteAt(int index, int symbolsToDelete)
{
    int startName = 6;
    int endName = startName + _name.length();

    if (index <= endName)
    {
        if (index < startName)
        {
            index = startName;
        }

        int realIndex = index - startName;

        std::string before = _name.substr(0, realIndex);

        std::string after = "";

        if (realIndex + symbolsToDelete < _name.length())
        {
            after = _name.substr(realIndex + symbolsToDelete);
        }

        _name = before + after;
    }
    else
    {
        int startEmail = _name.length() + 15;

        if (index < startEmail)
        {
            index = startEmail;
        }

        int realIndex = index - startEmail;

        if (realIndex > _email.length())
        {
            realIndex = _email.length();
        }

        std::string before = _email.substr(0, realIndex);
        
        std::string after = "";

        if (realIndex + symbolsToDelete < _email.length())
        {
            after = _email.substr(realIndex + symbolsToDelete);
        }

        _email = before + after;
    }
}

void ContactLine::copy(int index, int symbolsToCopy, std::string& clipboard)
{
    int startName = 6;
    int endName = startName + _name.length();

    if (index <= endName)
    {
        if (index < startName)
        {
            index = startName;
        }

        int realIndex = index - startName;

        if (realIndex + symbolsToCopy > _name.length())
        {
            symbolsToCopy = _name.length() - realIndex;
        }

        clipboard = _name.substr(realIndex, symbolsToCopy);
    }
    else
    {
        int startEmail = _name.length() + 15;

        if (index < startEmail)
        {
            index = startEmail;
        }

        int realIndex = index - startEmail;

        if (realIndex > _email.length())
        {
            realIndex = _email.length();
        }

        if (realIndex + symbolsToCopy > _email.length())
        {
            symbolsToCopy = _email.length() - realIndex;
        }

        clipboard = _email.substr(realIndex, symbolsToCopy);
    }
}

std::vector<int> ContactLine::search(std::string search)
{
    std::vector<int> results;

    int positionName = _name.find(search, 0);
    int positionEmail = _email.find(search);

    while (positionName != std::string::npos)
    {
        results.push_back(positionName + 6);

        positionName = _name.find(search, positionName + search.length());
    }

    while (positionEmail != std::string::npos)
    {
        results.push_back(positionEmail + 16 + 6 + _name.length());

        positionEmail = _email.find(search, positionEmail + search.length());
    }
    
    return results;
}

Line* ContactLine::clone() const
{
    return new ContactLine(*this);
}