#include "cipher_manager.h"
#include <dlfcn.h>
#include <iostream>

CipherManager::CipherManager(const std::string& libPath) : _libPath(libPath) {}

std::string CipherManager::ProcessText(const std::string& text, const std::string& type, const std::string& key, bool isEncrypt)
{
    void* lib = dlopen("./libcipher.dylib", RTLD_LAZY);

    if (!lib)
    {
        std::cout << "Cannot open the dylib!" << std::endl;

        return "";
    }

    CreateFunction create = (CreateFunction)dlsym(lib, "create");

    EncryptFunction cipherEncrypt = (EncryptFunction)dlsym(lib, "cipherEncrypt");

    DecryptFunction cipherDecrypt = (DecryptFunction)dlsym(lib, "cipherDecrypt");

    FreeFunction freeCipher = (FreeFunction)dlsym(lib, "freeCipher");

    if (!create || !cipherEncrypt || !cipherDecrypt || !freeCipher)
    {
        std::cout << "Cannot find such functions in dylib!" << std::endl;

        dlclose(lib);

        return "";
    }

    std::string res = "";

    void* cipherObj = create(type.c_str(), key.c_str());

    if (cipherObj)
    {
        if (isEncrypt)
        {
            res = cipherEncrypt(cipherObj, text.c_str());
        }
        else
        {
            res = cipherDecrypt(cipherObj, text.c_str());
        }

        freeCipher(cipherObj);
    }

    dlclose(lib);

    return res;
}

std::string CipherManager::Encrypt(const std::string& text, const std::string& type, const std::string& key)
{
    return ProcessText(text, type, key, true);
}

std::string CipherManager::Decrypt(const std::string& text, const std::string& type, const std::string& key)
{
    return ProcessText(text, type, key, false);
}
