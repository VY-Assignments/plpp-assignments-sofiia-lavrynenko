#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node
{
    char* letters;
    int currLength;
    int capacity;
    struct Node* next;
} Node;

typedef struct LList
{
    Node* head;
    Node* tail;
} LList;

LList* CreateList()
{
    LList* list = (LList*)malloc(sizeof(LList));

    list -> head = NULL;

    list -> tail = list -> head;

    return list;
}

void DestroyList(LList* list)
{
    Node* curr = list -> head;

    Node* afterCurr = NULL;

    while (curr != NULL)
    {
        afterCurr = curr -> next;

        free(curr -> letters);

        free(curr);

        curr = afterCurr;
    }

    free(list);
}

Node* CreateNewLine(LList* list)
{
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode)
    {
        printf("There is a problem. Cannot allocate memory for this node. \n");
        printf("\n");

        exit(1);
    }

    newNode -> capacity = 20;

    newNode -> letters = (char*)malloc(sizeof(char) * newNode -> capacity);

    if(!newNode -> letters)
    {
        printf("There is a problem. Cannot allocate memory for this line. \n");
        printf("\n");

        exit(1);
    }

    newNode -> letters[0] = '\0';

    newNode -> currLength = 0;
    newNode -> next = NULL;

    if (list -> head == NULL)
    {
        list -> head = newNode;
        list -> tail = list -> head;

        return newNode;
    }

    list -> tail -> next = newNode;

    list -> tail = newNode;

    return newNode;
}

void IncreaseLineSize(Node* line)
{
    line -> capacity = line -> capacity * 2;
    line -> letters = realloc(line -> letters, line -> capacity);
}

void AppendText(LList* list)
{
    Node* currLine = list -> tail;

    printf("Please, enter text to append: \n");

    int tempChar = 0;

    while (true)
    {
        tempChar = getchar();

        if (tempChar == '\n' || tempChar == EOF)
        {
            break;
        }

        if (currLine -> currLength == currLine -> capacity - 1)
        {
            IncreaseLineSize(currLine);
        }

        currLine -> letters[currLine -> currLength] = tempChar;

        currLine -> currLength++;
    }

    currLine -> letters[currLine -> currLength] = '\0';
}

void SaveToFile(LList* list)
{
    char fileName[30];

    printf("Please, enter the file name: \n");

    fgets(fileName, sizeof(fileName), stdin);

    int length = 0;

    while (fileName[length] != '\0')
    {
        if (fileName[length] == '\n')
        {
            fileName[length] = '\0';
            break;
        }

        length++;
    }

    FILE* file;

    file = fopen(fileName, "w");
                
    if (file != NULL)
    {
        if (list -> head == NULL)
        {
            printf("There is nothing to save.");

            fclose(file);

            return;
        }

        Node* curr = list -> head;

        while (curr != NULL)
        {
            fprintf(file, "%s\n", curr -> letters);
                        
            curr = curr -> next;
        }

        fclose(file);

        printf("Text successfully saved.");
    }
}

void LoadFromFile(LList* list)
{
    char fileName[30];

    printf("Please, enter the file name: \n");

    fgets(fileName, sizeof(fileName), stdin);

    int length = 0;

    while (fileName[length] != '\0')
    {
        if (fileName[length] == '\n')
        {
            fileName[length] = '\0';
            break;
        }

        length++;
    }

    FILE* file;

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("There is an error while opening the file.");

        return;
    }

    Node* curr = list -> head;

    while (curr != NULL)
    {
        Node* nextNode = curr -> next;

        free(curr -> letters);

        free(curr);

        curr = nextNode;
    }

    list -> head = NULL;

    list -> tail = list -> head;

    int tempChar = 0;

    Node* currLine = CreateNewLine(list);

    while ((tempChar = fgetc(file)) != EOF)
    {
        if (tempChar == '\n')
        {
            currLine = CreateNewLine(list);
        }
        else
        {
            if (currLine -> currLength == currLine -> capacity - 1)
            {
                IncreaseLineSize(currLine);
            }

            currLine -> letters[currLine -> currLength] = tempChar;

            currLine -> currLength++;
        }
    }

    fclose(file);

    printf("Text successfully loaded.");
}

void PrintToConsole(LList* list)
{
    printf("\n");

    Node* curr = list -> head;

    while (curr != NULL)
    {
        printf("%s \n", curr -> letters);

        curr = curr -> next;
    }
}

void InsertAt(LList* list)
{
    // line and symbol indexes block

    printf("Please, enter line index: \n");

    int lineIndexInput;
    scanf("%d", &lineIndexInput);

    getchar();

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    if (curr == NULL)
    {
        printf("Line index out of bounds.");

        return;
    }

    printf("Please, enter symbol index: \n");

    int symbolIndexInput;
    scanf("%d", &symbolIndexInput);

    getchar();

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // insertion temporary buffer block

    printf("Please, enter text to insert: \n");

    int insertionCapacity = 5;

    char* insertionBuff = (char*)malloc(sizeof(char) * insertionCapacity);

    int insertionLength = 0;

    int tempChar = 0;

    while (true)
    {
        tempChar = getchar();

        if (tempChar == '\n' || tempChar == EOF)
        {
            break;
        }

        if (insertionLength >= insertionCapacity - 1)
        {
            insertionCapacity *= 2;
            insertionBuff = realloc(insertionBuff, insertionCapacity);
        }

        insertionBuff[insertionLength] = tempChar;

        insertionLength++;
    }

    insertionBuff[insertionLength] = '\0';

    // insertion block

    int newLineLength = curr -> currLength + insertionLength;

    while (newLineLength >= curr -> capacity)
    {
        IncreaseLineSize(curr);
    }

    for (int i = curr -> currLength; i >= symbolIndexInput; i--)
    {
        curr -> letters[i + insertionLength] = curr -> letters[i];
    }

    for (int i = 0; i < insertionLength; i++)
    {
        curr -> letters[symbolIndexInput + i] = insertionBuff[i];
    }

    curr -> currLength = newLineLength;

    free(insertionBuff);
}

void SearchByWord(LList* list)
{
    Node* currLine = list -> head;

    if (currLine == NULL)
    {
        printf("There is no text.");

        return;
    }
    
    char search[30];

    printf("Please, enter what to search for: \n");

    fgets(search, sizeof(search), stdin);

    int length = 0;

    while (search[length] != '\0')
    {
        if (search[length] == '\n')
        {
            search[length] = '\0';
            break;
        }

        length++;
    }

    bool found = false;

    int lineIndex = 0;

    while (currLine != NULL)
    {
        char* foundPosition = strstr(currLine -> letters, search);

        while (foundPosition != NULL)
        {
            int symbolIndex = foundPosition - currLine -> letters; // a char is 1 byte, so it's basically 1005 - 1000 = 5

            printf("Found at: %3d %3d \n", lineIndex, symbolIndex);

            found = true;

            foundPosition = strstr(foundPosition + 1, search);
        }

        currLine = currLine -> next;

        lineIndex++;
    }

    if (!found)
    {
        printf("Nothing found.");
    }
}

void DeleteAt(LList* list)
{
    // line and symbol indexes block

    printf("Please, enter line index: \n");

    int lineIndexInput;
    scanf("%d", &lineIndexInput);

    getchar();

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    if (curr == NULL)
    {
        printf("Line index out of bounds.");

        return;
    }

    printf("Please, enter symbol index: \n");

    int symbolIndexInput;
    scanf("%d", &symbolIndexInput);

    getchar();

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // get symbols to delete block

    printf("Enter number of symbols to delete: \n");

    int symbolsToDelete;
    scanf("%d", &symbolsToDelete);

    if (symbolIndexInput + symbolsToDelete > curr -> currLength)
    {
        symbolsToDelete = curr -> currLength - symbolIndexInput;
    }

    int restOfSymbols = curr -> currLength - symbolIndexInput - symbolsToDelete;

    for (int i = 0; i < restOfSymbols; i++)
    {
        int readFrom = symbolIndexInput + symbolsToDelete + i;
        int writeTo = symbolIndexInput + i;

        curr -> letters[writeTo] = curr -> letters[readFrom];
    }

    curr -> currLength = curr -> currLength - symbolsToDelete;

    curr -> letters[curr -> currLength] = '\0';
}

void InsertWithReplace(LList* list)
{
    // line and symbol indexes block
    
    printf("Please, enter line index: \n");

    int lineIndexInput;
    scanf("%d", &lineIndexInput);

    getchar();

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    if (curr == NULL)
    {
        printf("Line index out of bounds.");

        return;
    }

    printf("Please, enter symbol index: \n");

    int symbolIndexInput;
    scanf("%d", &symbolIndexInput);

    getchar();

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // insertion temporary bufffer block

    printf("Please, enter text to insert: \n");

    int insertionCapacity = 5;

    char* insertionBuff = (char*)malloc(sizeof(char) * insertionCapacity);

    int insertionLength = 0;

    int tempChar = 0;

    while (true)
    {
        tempChar = getchar();

        if (tempChar == '\n' || tempChar == EOF)
        {
            break;
        }

        if (insertionLength >= insertionCapacity - 1)
        {
            insertionCapacity *= 2;
            insertionBuff = realloc(insertionBuff, insertionCapacity);
        }

        insertionBuff[insertionLength] = tempChar;

        insertionLength++;
    }

    // insertion with replacement block

    int newLength = symbolIndexInput + insertionLength;

    while (newLength > curr -> capacity)
    {
        IncreaseLineSize(curr);
    }

    for (int i = 0; i < insertionLength; i++)
    {
        curr -> letters[symbolIndexInput + i] = insertionBuff[i];
    }

    if (newLength > curr -> currLength)
    {
        curr -> currLength = newLength;
        curr -> letters[newLength] = '\0';
    }

    free(insertionBuff);
}

int main()
{
    bool isRunning = true;

    LList* list = CreateList();

    Node* head = CreateNewLine(list);

    while (isRunning)
    {
        printf("\n");
        printf("*** \n");
        printf("\n");
        printf("1 - append text symbols to the end \n");
        printf("2 - start a new line \n");
        printf("3 - save the text to the file \n");
        printf("4 - load the text from the file \n");
        printf("5 - print current text to the console \n");
        printf("6 - insert text by line and symbol index \n");
        printf("7 - search by word \n");
        printf("8 - delete by indexes and number of symbols \n");
        printf("9 - undo \n");
        printf("10 - redo \n");
        printf("11 - cut by indexes and number of symbols \n");
        printf("12 - paste by indexes \n");
        printf("13 - copy by indexes and number of symbols \n");
        printf("14 - insert with replacement \n");
        printf("15 - exit \n");
        printf("\n");
        printf("**** \n");
        printf("\n");
        printf("> choose the command: ");
        
        int input;
        scanf("%d", &input);

        getchar();

        switch (input)
        {
            case 1: 
            {
                AppendText(list);

                break;
            }

            case 2: 
            {
                CreateNewLine(list);

                printf("New line is started. \n");

                break;
            }

            case 3: 
            {
                SaveToFile(list);

                break;
            }

            case 4: 
            {
                LoadFromFile(list);

                break;
            }

            case 5: 
            {
                PrintToConsole(list);

                break;
            }

            case 6: 
            {
                InsertAt(list);

                break;
            }

            case 7: 
            {
                SearchByWord(list);

                break;
            }

            case 8:
            {
                DeleteAt(list);
                
                break;
            }

            case 9:
            {
                printf("Command is not implemented. \n");
                break;
            }

            case 10:
            {
                printf("Command is not implemented. \n");
                break;
            }

            case 11:
            {
                printf("Command is not implemented. \n");
                break;
            }

            case 12:
            {
                printf("Command is not implemented. \n");
                break;
            }

            case 13:
            {
                printf("Command is not implemented. \n");
                break;
            }

            case 14:
            {
                InsertWithReplace(list);

                break;
            }

            case 15: 
            {
                printf("Exiting the program. \n");
                printf("\n");
                DestroyList(list);
                isRunning = false;
                break;
            }

            default: 
            {
                printf("Incorrect input. \n");
                printf("\n");
                break;
            }
        }
    }
}
