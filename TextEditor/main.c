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

typedef struct CLipboard
{
    char* clipboard;
    int capacity;
    int currLength; 
} Clipboard;

typedef struct HistoryNode
{
    LList* version;
    struct HistoryNode* next;
} HistoryNode;

typedef struct HistoryStack
{
    HistoryNode* top;
} HistoryStack;

typedef struct Cursor
{
    int lineIndex;
    int symbolIndex;
} Cursor;

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

LList* CopyLList(LList* orig)
{
    LList* newList = CreateList();

    Node* currOrig = orig -> head;

    while (currOrig != NULL)
    {
        Node* newNode = (Node*)malloc(sizeof(Node));

        newNode -> capacity = currOrig -> capacity;

        newNode -> currLength = currOrig -> currLength;

        newNode -> letters = (char*)malloc(sizeof(char) * newNode -> capacity);

        for (int i = 0; i < currOrig -> currLength; i++)
        {
            newNode -> letters[i] = currOrig -> letters[i];
        }

        newNode -> next = NULL;

        if (newList -> head == NULL)
        {
            newList -> head = newNode;
            newList -> tail = newNode;
        }

        else
        {
            newList -> tail -> next = newNode;
            newList -> tail = newNode;
        }

        currOrig = currOrig -> next;
    }

    return newList;
}

HistoryStack* CreateHistoryStack()
{
    HistoryStack* history = (HistoryStack*)malloc(sizeof(HistoryStack));

    history -> top = NULL;

    return history;
}

HistoryNode* CreateNewHistoryNode(HistoryStack* history, LList* list)
{
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));

    newNode -> version = CopyLList(list);

    newNode -> next = history -> top;

    history -> top = newNode;

    return newNode;
}

void DestroyHistoryStack(HistoryStack* history)
{
    HistoryNode* curr = history -> top;

    while (curr != NULL)
    {
        DestroyList(curr -> version);

        HistoryNode* afterCurr = curr -> next;

        free(curr);

        curr = afterCurr;
    }

    free(history);
}

void ClearRedoStack(HistoryStack* forRedo)
{
    if (forRedo == NULL)
    {
        return;
    }

    HistoryNode* curr = forRedo -> top;

    while (curr != NULL)
    {
        DestroyList(curr -> version);

        HistoryNode* afterCurr = curr -> next;

        free(curr);

        curr = afterCurr;
    }

    forRedo -> top = NULL;
}

Node* CreateNewLine(LList* list, Cursor* cursor)
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

    cursor -> lineIndex ++;
    cursor -> symbolIndex = 0;

    return newNode;
}

void IncreaseLineSize(Node* line)
{
    line -> capacity = line -> capacity * 2;
    line -> letters = realloc(line -> letters, line -> capacity);
}

void AppendText(LList* list, Cursor* cursor)
{
    Node* currLine = list -> tail;

    printf("Please, enter text to append: \n");

    int tempChar = 0;

    int symbolsCount = 0;

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

        symbolsCount++;
    }

    currLine -> letters[currLine -> currLength] = '\0';

    cursor -> symbolIndex += symbolsCount;
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

void LoadFromFile(LList* list, Cursor* cursor)
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

    Node* currLine = CreateNewLine(list, cursor);

    while ((tempChar = fgetc(file)) != EOF)
    {
        if (tempChar == '\n')
        {
            currLine = CreateNewLine(list, cursor);
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

void InsertAt(LList* list, Cursor* cursor)
{
    // line and symbol indexes block

    int lineIndexInput = cursor -> lineIndex;

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    int symbolIndexInput = cursor -> symbolIndex;

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

    cursor -> symbolIndex += insertionLength;

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

void Cut(LList* list, Clipboard* clipboard, Cursor* cursor)
{
    // line and symbol indexes block

    int lineIndexInput = cursor -> lineIndex;

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    int symbolIndexInput = cursor -> symbolIndex;

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // get symbols to copy block

    printf("Enter number of symbols to cut: \n");

    int symbolsToCut;
    scanf("%d", &symbolsToCut);

    // copy block

    if (symbolIndexInput + symbolsToCut > curr -> currLength)
    {
        symbolsToCut = curr -> currLength - symbolIndexInput;
    }

    if (clipboard -> capacity < symbolsToCut + 1)
    {
        clipboard -> capacity = (symbolsToCut + 1) * 2;

        clipboard -> clipboard = realloc(clipboard -> clipboard, sizeof(char) * clipboard -> capacity);
    }

    int clipboardIndex = 0;

    for (int i = symbolIndexInput; i < symbolsToCut + symbolIndexInput; i++)
    {
        clipboard -> clipboard[clipboardIndex] = curr -> letters[i];

        clipboardIndex++;
    }

    clipboard -> clipboard[symbolsToCut] = '\0';

    clipboard -> currLength = symbolsToCut;

    // delete block

    int restOfSymbols = curr -> currLength - symbolIndexInput - symbolsToCut;

    for (int i = 0; i < restOfSymbols; i++)
    {
        int readFrom = symbolIndexInput + symbolsToCut + i;
        int writeTo = symbolIndexInput + i;

        curr -> letters[writeTo] = curr -> letters[readFrom];
    }

    curr -> currLength = curr -> currLength - symbolsToCut;

    curr -> letters[curr -> currLength] = '\0';

    printf("Cutted. \n");
}

void Paste(LList* list, Clipboard* clipboard, Cursor* cursor)
{
    // check if there's something to paste block

    if (clipboard == NULL || clipboard -> currLength == 0)
    {
        printf("The clipboard is empty. Nothing to paste. \n");

        return;
    }

    // line and symbol indexes block

    int lineIndexInput = cursor -> lineIndex;

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    int symbolIndexInput = cursor -> symbolIndex;

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // paste block

    int newLineLength = curr -> currLength + clipboard -> currLength;

    while (newLineLength >= curr -> capacity)
    {
        IncreaseLineSize(curr);
    }

    for (int i = curr -> currLength; i >= symbolIndexInput; i--)
    {
        curr -> letters[i + clipboard -> currLength] = curr -> letters[i];
    }

    for (int i = 0; i < clipboard -> currLength; i++)
    {
        curr -> letters[symbolIndexInput + i] = clipboard -> clipboard[i];
    }

    curr -> currLength = newLineLength;

    cursor -> symbolIndex += clipboard -> currLength;

    printf("Pasted. \n");
}

void Copy(LList* list, Clipboard* clipboard, Cursor* cursor)
{
    // line and symbol indexes block

    int lineIndexInput = cursor -> lineIndex;

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    int symbolIndexInput = cursor -> symbolIndex;

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // get symbols to copy block

    printf("Enter number of symbols to copy: \n");

    int symbolsToCopy;
    scanf("%d", &symbolsToCopy);

    // copy block

    if (symbolIndexInput + symbolsToCopy > curr -> currLength)
    {
        symbolsToCopy = curr -> currLength - symbolIndexInput;
    }

    if (clipboard -> capacity < symbolsToCopy + 1)
    {
        clipboard -> capacity = (symbolsToCopy + 1) * 2;

        clipboard -> clipboard = realloc(clipboard -> clipboard, sizeof(char) * clipboard -> capacity);
    }

    int clipboardIndex = 0;

    for (int i = symbolIndexInput; i < symbolsToCopy + symbolIndexInput; i++)
    {
        clipboard -> clipboard[clipboardIndex] = curr -> letters[i];

        clipboardIndex++;
    }

    clipboard -> clipboard[symbolsToCopy] = '\0';

    clipboard -> currLength = symbolsToCopy;

    printf("Copied. \n");
}

void DeleteAt(LList* list, Cursor* cursor)
{
    // line and symbol indexes block

    int lineIndexInput = cursor -> lineIndex;

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    int symbolIndexInput = cursor -> symbolIndex;

    if (symbolIndexInput < 0 || symbolIndexInput > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    // get symbols to delete block

    printf("Enter number of symbols to delete: \n");

    int symbolsToDelete;
    scanf("%d", &symbolsToDelete);

    // delete block

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

    cursor -> symbolIndex -= symbolsToDelete;
}

void Undo(LList** list, HistoryStack* history, HistoryStack* forRedo)
{
    if (history == NULL || history -> top == NULL)
    {
        printf("Nothing to undo. History is empty. \n");

        return;
    }

    CreateNewHistoryNode(forRedo, *list);

    HistoryNode* temp = history -> top;

    DestroyList(*list);

    *list = temp -> version;

    history -> top = history -> top -> next;

    free(temp);

    printf("Undo is finished. \n");
}

void Redo(LList** list, HistoryStack* forRedo, HistoryStack* history)
{
    if (forRedo == NULL || forRedo -> top == NULL)
    {
        printf("Nothing to redo. History is empty. \n");

        return;
    }

    CreateNewHistoryNode(history, *list);

    HistoryNode* temp = forRedo -> top;

    DestroyList(*list);

    *list = temp -> version;

    forRedo -> top = forRedo -> top -> next;

    free(temp);

    printf("Redo is finished. \n");
}

void InsertWithReplace(LList* list, Cursor* cursor)
{
    // line and symbol indexes block
    
    int lineIndexInput = cursor -> lineIndex;

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < lineIndexInput)
    {
        curr = curr -> next;

        count++;
    }

    int symbolIndexInput = cursor -> symbolIndex;

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
    }

    curr -> letters[curr -> currLength] = '\0';

    cursor -> symbolIndex += insertionLength;

    free(insertionBuff);
}

void MoveCursor(LList* list, Cursor* cursor)
{
    printf("Enter new line index: \n");

    int newLineIndex;
    scanf("%d", &newLineIndex);

    getchar();

    Node* curr = list -> head;

    int count = 0;

    while (curr != NULL && count < newLineIndex)
    {
        curr = curr -> next;

        count++;
    }

    if (curr == NULL)
    {
        printf("Line index out of bounds.");

        return;
    }

    printf("Enter new symbol index: \n");

    int newSymbolIndex;
    scanf("%d", &newSymbolIndex);

    getchar();

    if (newSymbolIndex < 0 || newSymbolIndex > curr -> currLength)
    {
        printf("Symbol index out of bounds.");

        return;
    }

    cursor -> lineIndex = newLineIndex;

    cursor -> symbolIndex = newSymbolIndex;
}

int main()
{
    bool isRunning = true;

    LList* list = CreateList();

    Cursor cursor;

    cursor.lineIndex = 0;
    cursor.symbolIndex = 0;

    Node* head = CreateNewLine(list, &cursor);

    Clipboard clipboard;

    clipboard.capacity = 5;
    clipboard.currLength = 0;
    clipboard.clipboard = (char*)malloc(sizeof(char) * clipboard.capacity);

    HistoryStack* history = CreateHistoryStack();

    HistoryStack* forRedo = CreateHistoryStack();

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
        printf("15 - move cursor by indexes \n");
        printf("16 - exit \n");
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
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                AppendText(list, &cursor);

                break;
            }

            case 2: 
            {
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                CreateNewLine(list, &cursor);

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
                DestroyHistoryStack(history);
                history = CreateHistoryStack();

                DestroyHistoryStack(forRedo);
                forRedo = CreateHistoryStack();

                LoadFromFile(list, &cursor);

                break;
            }

            case 5: 
            {
                PrintToConsole(list);

                break;
            }

            case 6: 
            {
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                InsertAt(list, &cursor);

                break;
            }

            case 7: 
            {
                SearchByWord(list);

                break;
            }

            case 8:
            {
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                DeleteAt(list, &cursor);
                
                break;
            }

            case 9:
            {
                Undo(&list, history, forRedo);

                break;
            }

            case 10:
            {
                Redo(&list, forRedo, history);

                break;
            }

            case 11:
            {
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                Cut(list, &clipboard, &cursor);

                break;
            }

            case 12:
            {
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                Paste(list, &clipboard, &cursor);

                break;
            }

            case 13:
            {
                Copy(list, &clipboard, &cursor);

                break;
            }

            case 14:
            {
                ClearRedoStack(forRedo);

                CreateNewHistoryNode(history, list);

                InsertWithReplace(list, &cursor);

                break;
            }

            case 15:
            {
                MoveCursor(list, &cursor);

                break;
            }

            case 16: 
            {
                printf("Exiting the program. \n");
                printf("\n");

                DestroyList(list);

                free(clipboard.clipboard);

                DestroyHistoryStack(history);
                DestroyHistoryStack(forRedo);

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
