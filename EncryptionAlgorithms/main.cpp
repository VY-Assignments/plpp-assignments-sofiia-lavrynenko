#include <iostream>
#include <dlfcn.h>

typedef void* (*create_function)(const char*, const char*);
typedef const char* (*cipherEncrypt_function)(void*, const char*);
typedef const char* (*cipherDecrypt_function)(void*, const char*);
typedef void (*freeCipher_function)(void*);

int main()
{
    void* lib = dlopen("./libcipher.dylib", RTLD_LAZY);

    if (!lib)
    {
        std::cout << "Cannot open the dylib!" << std::endl;

        return 1;
    }

    create_function createCipher = (create_function)dlsym(lib, "create");

    cipherEncrypt_function encryptCipher = (cipherEncrypt_function)dlsym(lib, "cipherEncrypt");

    cipherDecrypt_function decryptCipher = (cipherDecrypt_function)dlsym(lib, "cipherDecrypt");

    freeCipher_function freeCipher = (freeCipher_function)dlsym(lib, "freeCipher");

    if (!createCipher || !encryptCipher || !decryptCipher || !freeCipher)
    {
        std::cout << "Cannot find such functions in dylib!" << std::endl;

        dlclose(lib);

        return 1;
    }

    bool isRunning = true;

    const char* encryp = nullptr;

    while (isRunning)
    {
        std::cout << "Welcome!" << std::endl;
        std::cout << "1 - use caesar cipher" << std::endl;
        std::cout << "2 - use vigenere cipher" << std::endl;
        std::cout << "3 - exit" << std::endl;

        std::cout << "Choose your option:" << std::endl;

        int input;

        if (!(std::cin >> input))
        {
            std::cout << "You need to enter a number!" << std::endl;
            std::cout << std::endl;

            std::cin.clear();

            std::cin.ignore(10000, '\n');

            continue;
        }

        std::cout << std::endl;

        switch (input)
        {
            case 1:
                {
                    std::cout << "You've chosen caesar cipher!" << std::endl;
                    std::cout << std::endl;

                    std::cout << "Please, enter your key:" << std::endl;
                    std::string key;
                    std::cin >> key;
                    std::cout << std::endl;

                    std::cout << "1 - encrypt" << std::endl;
                    std::cout << "2 - decrypt" << std::endl;
                    std::cout << "Choose your next option:" <<std::endl;

                    int inputC;

                    std::cin >> inputC;
                    std::cout << std::endl;

                    void* caesar = createCipher("C", key.c_str());

                    if (caesar)
                    {
                        if (inputC == 1)
                        {
                            std::cout << "Please, enter your text:" << std::endl;
                            std::string origText;
                            
                            std::cin.ignore();
                            std::getline(std::cin, origText);
                            std::cout << std::endl;

                            encryp = encryptCipher(caesar, origText.c_str());

                            std::cout << "Encrypted: " << encryp << std::endl;
                            std::cout << std::endl;
                        }
                        
                        else if (inputC == 2)
                        {
                            if (encryp == nullptr || encryp[0] == '\0')
                            {
                                std::cout << "You need to encrypt something first." << std::endl;
                                std::cout << std::endl;
                            }

                            else
                            {
                                const char* decryp = decryptCipher(caesar, encryp);

                                std::cout << "Decrypted: " << decryp << std::endl;
                                std::cout << std::endl;
                            }
                        }

                        else
                        {
                            std::cout << "Invalid input." << std::endl;
                        }

                        freeCipher(caesar);
                    }

                    break;
                }
            
            case 2:
                {
                    std::cout << "You've chosen vigenere cipher!" << std::endl;
                    std::cout << std::endl;

                    std::cout << "Please, enter your key:" << std::endl;
                    std::string key;
                    std::cin >> key;
                    std::cout << std::endl;

                    std::cout << "1 - encrypt" << std::endl;
                    std::cout << "2 - decrypt" << std::endl;
                    std::cout << "Choose your next option:" <<std::endl;

                    int inputV;

                    std::cin >> inputV;
                    std::cout << std::endl;

                    void* vigenere = createCipher("V", key.c_str());

                    if (vigenere)
                    {
                        if (inputV == 1)
                        {
                            std::cout << "Please, enter your text:" << std::endl;
                            std::string origText;
                            
                            std::cin.ignore();
                            std::getline(std::cin, origText);
                            std::cout << std::endl;

                            encryp = encryptCipher(vigenere, origText.c_str());

                            std::cout << "Encrypted: " << encryp << std::endl;
                            std::cout << std::endl;
                        }
                        
                        else if (inputV == 2)
                        {
                            if (encryp == nullptr || encryp[0] == '\0')
                            {
                                std::cout << "You need to encrypt something first." << std::endl;
                                std::cout << std::endl;
                            }

                            else
                            {
                                const char* decryp = decryptCipher(vigenere, encryp);

                                std::cout << "Decrypted: " << decryp << std::endl;
                                std::cout << std::endl;
                            }
                        }

                        else
                        {
                            std::cout << "Invalid input." << std::endl;
                        }

                        freeCipher(vigenere);
                    }

                    break;
                }

            case 3:
                {
                    std::cout << "Exiting the program." << std::endl;
                
                    isRunning = false;

                    dlclose(lib);

                    break;
                }
            
            default:
                {
                    std::cout << "Invalid inout." << std::endl;

                    break;
                }
        }
    }
}