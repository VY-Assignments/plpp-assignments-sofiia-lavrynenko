#include "line.h"
#include <iostream>

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