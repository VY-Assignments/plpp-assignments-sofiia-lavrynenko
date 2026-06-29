#include <iostream>
#include "texteditor.h"
#include "cipher_manager.h"

int main()
{
    bool isRunning = true;

    TextEditor textEditor;

    while (isRunning)
    {   
        std::cout << "\n";
        std::cout << "Welcome to text editor! \n";
        std::cout << "1 - append text to the end \n";
        std::cout << "2 - save to file \n";
        std::cout << "3 - load from file \n";
        std::cout << "4 - print to the console \n";
        std::cout << "5 - move the cursor \n";
        std::cout << "6 - insert \n";
        std::cout << "7 - insert with replacement \n";
        std::cout << "8 - delete \n";
        std::cout << "9 - copy \n";
        std::cout << "10 - paste \n";
        std::cout << "11 - cut \n";
        std::cout << "12 - search by word \n";
        std::cout << "13 - undo \n";
        std::cout << "14 - redo \n";
        std::cout << "15 - encrypt the text \n";
        std::cout << "16 - decrypt the text \n";
        std::cout << "17 - save to file encrypted \n";
        std::cout << "18 - load from file decrypted \n";
        std::cout << "19 - change checkbox status \n";
        std::cout << "20 - exit \n";
        std::cout << "\n";
        std::cout << "Choose your option: ";

        int input;

        if (!(std::cin >> input))
        {
            std::cout << "Invalid input. \n";

            std::cin.clear();
            std::cin.ignore(10000, '\n');

            continue;
        }

        std::cout << "\n";

        std::cin.ignore();

        switch (input)
        {
            case 1:
                {
                    std::cout << "Please, choose the line type: \n";
                    std::cout << "1 - text line \n";
                    std::cout << "2 - checkbox line \n";
                    std::cout << "3 - contact line \n";
                    std::cout << "\n";
                    std::cout << "Choose your option: ";

                    int lineInput;

                    std::cin >> lineInput;
                    std::cout << "\n";

                    std::cin.ignore();

                    switch (lineInput)
                    {
                        case 1:
                            {
                                std::cout << "Please, enter the text: ";

                                std::string text;
                                std::getline (std::cin, text);

                                std::cout << "\n";

                                textEditor.AddTextLine(text);

                                break;
                            }
                        
                        case 2:
                            {
                                std::cout << "Please, enter the text: ";

                                std::string text;
                                std::getline (std::cin, text);

                                std::cout << "\n";

                                int checkInput;

                                std::cout << "Is it done (1 or 0): ";

                                std::cin >> checkInput;

                                std::cout << "\n";

                                std::cin.ignore();

                                bool check;

                                if (checkInput == 1)
                                {
                                    check = true;
                                }
                                else if (checkInput == 0)
                                {
                                    check = false;
                                }
                                else
                                {
                                    std::cout << "Invalid input. \n";
                                    check = false;
                                }

                                textEditor.AddCheckLine(text, check);

                                break;
                            }
                        
                        case 3:
                            {
                                std::cout << "Please, enter the name: ";

                                std::string name;
                                std::getline (std::cin, name);

                                std::cout << "\n";

                                std::cout << "Please, enter the email: ";

                                std::string email;
                                std::getline (std::cin, email);

                                std::cout << "\n";

                                textEditor.AddContactLine(name, email);

                                break;
                            }
                        
                        default:
                            {
                                std::cout << "Invalid input. \n";
                                
                                break;
                            }
                    }

                    break;
                }
            
            case 2:
                {
                    std::cout << "Please, enter the file name: ";

                    std::string filename;
                    std::getline(std::cin, filename);

                    std::cout << "\n";

                    if (filename.empty())
                    {
                        std::cout << "You need to enter a filename. \n";
                    }
                    else
                    {
                        textEditor.SaveToFile(filename);
                    }

                    break;
                }

            case 3:
                {
                    std::cout << "Please, enter the file name: ";

                    std::string filename;
                    std::getline(std::cin, filename);

                    std::cout << "\n";

                    if (filename.empty())
                    {
                        std::cout << "You need to enter a filename. \n";
                    }
                    else
                    {
                        textEditor.LoadFromFile(filename);
                    }

                    break;
                }

            case 4:
                {
                    textEditor.PrintToConsole();

                    break;
                }

            case 5:
                {
                    std::cout << "Please, enter the line index: ";

                    int line;
                    std::cin >> line;

                    std::cout << "\n";

                    std::cout << "Please, enter the symbol index: ";

                    int symbol;
                    std::cin >> symbol;

                    std::cout << "\n";

                    std::cin.ignore();

                    textEditor.MoveCursor(line, symbol);

                    break;
                }
            
            case 6:
                {
                    std::cout << "Please, enter the text to insert: ";

                    std::string text;
                    std::getline (std::cin, text);

                    std::cout << "\n";

                    textEditor.InsertAt(text);

                    break;
                }

            case 7:
                {
                    std::cout << "Please, enter the text to insert with replacement: ";

                    std::string text;
                    std::getline (std::cin, text);

                    std::cout << "\n";

                    textEditor.InsertWithReplace(text);

                    break;
                }
            
            case 8:
                {
                    std::cout << "Please, enter the number of symbols to delete: ";

                    int number;
                    std::cin >> number;

                    std::cout << "\n";

                    std::cin.ignore();

                    textEditor.DeleteAt(number);

                    break;
                }

            case 9:
                {
                    std::cout << "Please, enter the number of symbols to copy: ";

                    int number;
                    std::cin >> number;

                    std::cout << "\n";

                    std::cin.ignore();

                    textEditor.Copy(number);

                    break;
                }
            
            case 10:
                {
                    textEditor.Paste();

                    break;
                }

            case 11:
                {
                    std::cout << "Please, enter the number of symbols to cut: ";

                    int number;
                    std::cin >> number;

                    std::cout << "\n";

                    std::cin.ignore();

                    textEditor.Cut(number);

                    break;
                }
            
            case 12:
                {
                    std::cout << "Please, enter the word to search by: ";

                    std::string word;
                    std::getline (std::cin, word);

                    std::cout << "\n";

                    textEditor.SearchByWord(word);

                    break;
                }
            
            case 13:
                {
                    textEditor.Undo();

                    break;
                }
            
            case 14:
                {
                    textEditor.Redo();

                    break;
                }
            
            case 15:
                {
                    std::cout << "Please, choose the cipher type: \n";
                    std::cout << "1 - caesar \n";
                    std::cout << "2 - vigenere \n";
                    std::cout << "\n";
                    std::cout << "Choose your option: ";

                    int cipherInput;
                    std::cin >> cipherInput;

                    std::cout << "\n";

                    std::cin.ignore();

                    switch (cipherInput)
                    {
                        case 1:
                            {
                                std::cout << "Please, enter the key: ";

                                int key;
                                std::cin >> key;

                                std::cout << "\n";

                                std::cin.ignore();

                                textEditor.EncryptAllText("C", std::to_string(key));

                                break;
                            }

                        case 2:
                            {
                                std::cout << "Please, enter the key: ";

                                std::string key;
                                std::getline(std::cin, key);

                                std::cout << "\n";

                                textEditor.EncryptAllText("V", key);

                                break;
                            }
                        
                        default: 
                            {
                                std::cout << "Invalid input. \n";

                                break;
                            }
                    }

                    break;
                }
            
            case 16:
                {
                    std::cout << "Please, choose the cipher type: \n";
                    std::cout << "1 - caesar \n";
                    std::cout << "2 - vigenere \n";
                    std::cout << "\n";
                    std::cout << "Choose your option: ";

                    int cipherInput;
                    std::cin >> cipherInput;

                    std::cout << "\n";

                    std::cin.ignore();

                    switch (cipherInput)
                    {
                        case 1:
                            {
                                std::cout << "Please, enter the key: ";

                                int key;
                                std::cin >> key;

                                std::cout << "\n";

                                std::cin.ignore();

                                textEditor.DecryptAllText("C", std::to_string(key));

                                break;
                            }

                        case 2:
                            {
                                std::cout << "Please, enter the key: ";

                                std::string key;
                                std::getline(std::cin, key);

                                std::cout << "\n";

                                textEditor.DecryptAllText("V", key);

                                break;
                            }
                        
                        default: 
                            {
                                std::cout << "Invalid input. \n";

                                break;
                            }
                    }

                    break;
                }

            case 17:
                {
                    std::cout << "Please, enter the file name: ";

                    std::string filename;
                    std::getline(std::cin, filename);

                    std::cout << "\n";

                    if (filename.empty())
                    {
                        std::cout << "You need to enter a filename. \n";

                        break;
                    }

                    std::cout << "Please, choose the cipher type: \n";
                    std::cout << "1 - caesar \n";
                    std::cout << "2 - vigenere \n";
                    std::cout << "\n";
                    std::cout << "Choose your option: ";

                    int cipherInput;
                    std::cin >> cipherInput;

                    std::cout << "\n";

                    std::cin.ignore();

                    switch (cipherInput)
                    {
                        case 1:
                            {
                                std::cout << "Please, enter the key: ";

                                int key;
                                std::cin >> key;

                                std::cout << "\n";

                                std::cin.ignore();

                                textEditor.SaveEncrypted(filename, "C", std::to_string(key));

                                break;
                            }

                        case 2:
                            {
                                std::cout << "Please, enter the key: ";

                                std::string key;
                                std::getline(std::cin, key);

                                std::cout << "\n";

                                textEditor.SaveEncrypted(filename, "V", key);

                                break;
                            }
                        
                        default: 
                            {
                                std::cout << "Invalid input. \n";

                                break;
                            }
                    }

                    break;
                }
            
            case 18:
                {
                    std::cout << "Please, enter the file name: ";

                    std::string filename;
                    std::getline(std::cin, filename);

                    std::cout << "\n";

                    if (filename.empty())
                    {
                        std::cout << "You need to enter a filename. \n";

                        break;
                    }

                    std::cout << "Please, choose the cipher type: \n";
                    std::cout << "1 - caesar \n";
                    std::cout << "2 - vigenere \n";
                    std::cout << "\n";
                    std::cout << "Choose your option: ";

                    int cipherInput;

                    std::cin >> cipherInput;

                    std::cout << "\n";

                    std::cin.ignore();

                    switch (cipherInput)
                    {
                        case 1:
                            {
                                std::cout << "Please, enter the key: ";

                                int key;
                                std::cin >> key;

                                std::cout << "\n";

                                std::cin.ignore();

                                textEditor.LoadEncrypted(filename, "C", std::to_string(key));

                                break;
                            }

                        case 2:
                            {
                                std::cout << "Please, enter the key: ";

                                std::string key;
                                std::getline(std::cin, key);

                                std::cout << "\n";

                                textEditor.LoadEncrypted(filename, "V", key);

                                break;
                            }
                        
                        default: 
                            {
                                std::cout << "Invalid input. \n";

                                break;
                            }
                    }

                    break;
                }

            case 19:
                {
                    textEditor.ChangeStatus();

                    break;
                }
            
            case 20:
                {
                    std::cout << "Exiting the program. \n";

                    isRunning = false;

                    break;
                }
        }
    }

    return 0;
}