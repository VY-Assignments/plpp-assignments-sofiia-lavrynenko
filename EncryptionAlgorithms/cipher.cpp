#include "cipher.h"
#include <iostream>
#include <string>

CaesarCipher::CaesarCipher(int key)
{
    _key = key;
}

std::string CaesarCipher::encrypt(const std::string &text)
{
    std::string res = "";

    int textLength = text.length();

    char ch;

    char base;

    for (int i = 0; i < textLength; i++)
    {
        ch = text[i];

        if (ch >= 'A' && ch <= 'Z')
        {
            base = 'A';

            res += base + ((ch - base + _key) % 26 + 26) % 26;
        }
        else if (ch >= 'a' && ch <= 'z')
        {
            base = 'a';

            res += base + ((ch - base + _key) % 26 + 26) % 26;
        }

        else
        {
            res += ch;
        }
    }

    return res;
}

std::string CaesarCipher::decrypt(const std::string &text)
{
    std::string res = "";

    int textLength = text.length();

    char ch;

    char base;

    for (int i = 0; i < textLength; i++)
    {
        ch = text[i];

        if (ch >= 'A' && ch <= 'Z')
        {
            base = 'A';

            res += base + ((ch - base - _key) % 26 + 26) % 26;
        }
        else if (ch >= 'a' && ch <= 'z')
        {
            base = 'a';

            res += base + ((ch - base - _key) % 26 + 26) % 26;
        }

        else
        {
            res += ch;
        }
    }

    return res;
}

VigenereCipher::VigenereCipher(std::string key)
{
    _key = key;
}

std::string VigenereCipher::encrypt(const std::string &text)
{
    std::string res = "";

    int keyLength = _key.length();

    int textLength = text.length();

    char ch;

    char base;

    int j = 0;

    for (int i = 0; i < textLength; i++)
    {
        ch = text[i];

        char keyCh = _key[j % keyLength];

        int keyShift = std::tolower(keyCh) - 'a';

        if (ch >= 'A' && ch <= 'Z')
        {
            base = 'A';

            res += base + ((ch - base + keyShift) % 26 + 26) % 26;

            j++;
        }

        else if (ch >= 'a' && ch <= 'z')
        {
            base = 'a';

            res += base + ((ch - base + keyShift) % 26 + 26) % 26;

            j++;
        }

        else
        {
            res += ch;
        }
    }

    return res;
}

std::string VigenereCipher::decrypt(const std::string &text)
{
    std::string res = "";

    int keyLength = _key.length();

    int textLength = text.length();

    char ch;

    char base;

    int j = 0;

    for (int i = 0; i < textLength; i++)
    {
        ch = text[i];

        char keyCh = _key[j % keyLength];

        int keyShift = std::tolower(keyCh) - 'a';

        if (ch >= 'A' && ch <= 'Z')
        {
            base = 'A';

            res += base + ((ch - base - keyShift) % 26 + 26) % 26;

            j++;
        }

        else if (ch >= 'a' && ch <= 'z')
        {
            base = 'a';

            res += base + ((ch - base - keyShift) % 26 + 26) % 26;

            j++;
        }

        else
        {
            res += ch;
        }
    }

    return res;
}