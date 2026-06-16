#include "cipher.h"
#include <iostream>
#include <string>
#include <sstream>

static std::string apiBuffEncryp;

static std::string apiBuffDecryp;

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

VernamCipher::VernamCipher(std::string key)
{
    _key = key;
}

std::string VernamCipher::encrypt(const std::string &text)
{
    int keyLength = _key.length();

    int textLength = text.length();

    if (keyLength < textLength)
    {
        std::cout << "Key must be the same or longer than text to encrypt." << std::endl;

        return "";
    }

    std::string res;

    for (int i = 0; i < textLength; i++)
    {
        int xorRes = text[i] ^ _key[i];

        res += std::to_string(xorRes);

        if (i < textLength - 1)
        {
            res += " ";
        }
    }

    return res;
}

std::string VernamCipher::decrypt(const std::string &text)
{
    std::string res = "";

    int xorRes;

    int i = 0;

    int j = 0;

    while (i < text.length())
    {
        int nextSp = text.find(' ', i);

        std::string number = text.substr(i, nextSp - i);

        xorRes = std::stoi(number);

        if (j >= _key.length())
        {
            std::cout << "Key must be the same or longer than text to encrypt." << std::endl;

            return "";
        }

        char origCh = xorRes ^ _key[j];

        res += origCh;

        j++;

        if (nextSp == std::string::npos)
        {
            break;
        }

        i = nextSp + 1;
    }

    return res;
}

extern "C"
{
    void* create(const char* type, const char* key)
    {
        std::string cipherType(type);
        std::string cipherKey(key);

        if (cipherType == "C")
        {
            int intKey = std::stoi(cipherKey);

            return new CaesarCipher(intKey);
        }

        else if (cipherType == "V")
        {
            return new VigenereCipher(cipherKey);
        }

        else if (cipherType == "Vern")
        {
            return new VernamCipher(cipherKey);
        }

        else
        {
            return nullptr;
        }
    }

    const char* cipherEncrypt(void* cipherType, const char* text)
    {
        if (cipherType == nullptr)
        {
            return "";
        }

        Cipher* curr = (Cipher*)cipherType;

        apiBuffEncryp = curr -> encrypt(text);

        return apiBuffEncryp.c_str();
    }

    const char* cipherDecrypt(void* cipherType, const char* text)
    {
        if (cipherType == nullptr)
        {
            return "";
        }

        Cipher* curr = (Cipher*)cipherType;

        apiBuffDecryp = curr -> decrypt(text);

        return apiBuffDecryp.c_str();
    }

    void freeCipher(void* cipherType)
    {
        if (cipherType != nullptr)
        {
            Cipher* curr = (Cipher*)cipherType;

            delete curr;
        }
    }
}