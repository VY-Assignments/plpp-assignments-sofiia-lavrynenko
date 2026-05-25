#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node
{
    char* letters;
    int currLength;
    int capacity;
    struct Node* next;
};

struct Node* CreateNewLine()
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

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
        printf("There is a problem. Cannot allocate memory for this char array. \n");
        printf("\n");

        exit(1);
    }

    newNode -> letters[0] = '\0';

    newNode -> currLength = 0;
    newNode -> next = NULL;

    return newNode;
}

void FreeAllList(struct Node* head)
{
    struct Node* current = head;

    while (current != NULL)
    {
        struct Node* next = current -> next;
        free(current -> letters);
        free(current);
        current = next;
    }
}

int main()
{
    bool isRunning = true;

    struct Node* head = CreateNewLine();
    struct Node* current = head;

    while (isRunning)
    {
        printf("1 - append text symbols to the end \n");
        printf("2 - start a new line \n");
        printf("3 - save the text to the file \n");
        printf("4 - load the text from the file \n");
        printf("5 - print current text to the console \n");
        printf("6 - insert text by line and symbol index \n");
        printf("7 - search by word \n");
        printf("8 - exit \n");
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
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 2: 
            {
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 3: 
            {
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 4: 
            {
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 5: 
            {
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 6: 
            {
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 7: 
            {
                printf("The command is not implemented. \n");
                printf("\n");
                break;
            }

            case 8: 
            {
                printf("Exiting the program. \n");
                printf("\n");
                FreeAllList(head);
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
