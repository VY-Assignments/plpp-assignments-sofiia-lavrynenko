#ifndef CIPHER_H
#define CIPHER_H

#include <string>

class Cipher 
{
    public:
        virtual std::string encrypt(const std::string &text) = 0;
        virtual std::string decrypt(const std::string &text) = 0;

        virtual ~Cipher() = default;
};

class CaesarCipher : public Cipher
{
    private: 
        int _key;

    public:
        CaesarCipher(int key);

        std::string encrypt(const std::string &text) override;
        std::string decrypt(const std::string &text) override;
};

class VigenereCipher : public Cipher
{
    private:
        std::string _key;
    
    public: 
        VigenereCipher(std::string key);

        std::string encrypt(const std::string &text) override;
        std::string decrypt(const std::string &text) override;
};

extern "C" 
{
    void* create(const char* type, const char* key);

    const char* cipherEncrypt(void* cipherType, const char* text);

    const char* cipherDecrypt(void* cipherType, const char* text);

    void freeCipher(void* cipherType);
}

#endif