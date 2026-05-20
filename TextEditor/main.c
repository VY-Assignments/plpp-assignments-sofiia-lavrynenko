#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    bool isRunning = true;

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