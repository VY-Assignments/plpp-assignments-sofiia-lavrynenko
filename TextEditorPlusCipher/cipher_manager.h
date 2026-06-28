#ifndef CIPHER_MANAGER_H
#define CIPHER_MANAGER_H

#include <string>

class CipherManager
{
    private:
        std::string _libPath;

        typedef void* (*CreateFunction)(const char*, const char*);
        typedef const char* (*EncryptFunction)(void*, const char*);
        typedef const char* (*DecryptFunction)(void*, const char*);
        typedef void (*FreeFunction)(void*);

        std::string ProcessText(const std::string& text, const std::string& type, const std::string& key, bool isEncrypt);

        public:
            CipherManager(const std::string& libPath = "./libcipher.dylib");

            std::string Encrypt(const std::string& text, const std::string& type, const std::string& key);
            std::string Decrypt(const std::string& text, const std::string& type, const std::string& key);
};

#endif