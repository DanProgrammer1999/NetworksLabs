#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "stack.h"
#include <ctype.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

int pipe_fd[2];
int server_id;

void terminate()
{
    printf("Exiting program\n");
    kill(getppid(), SIGKILL);
    exit(0);
}

void start_msg()
{
    char *message[12] =
        {
            "To control the stack you can use following commands:\n\n",
            "create - create a new stack \n",
            "empty - check is the stack is empty \n",
            "stack_size - output the size of the stack\n",
            "pop - display the top element and remove it\n",
            "peek - display the top element without removing it\n",
            "push(int data) - enter *without* any spaces \n",
            "display - output all elements in the stack \n",
            "help - get list of all commands \n",
            "exit - leave the program\n\n",
            "Please take into account that space symbols are not allowed \n",
            "Everything separated by space will be regarded as a separate command\n",
        };

    printf(ANSI_COLOR_YELLOW);
    for (int i = 0; i < 12; i++)
    {
        printf("%s", message[i]);
    }
}

int validate_push(char *input)
{
    if (input[strlen(input) - 1] != ')' || input[4] != '(' || strlen(input) <= 6)
    {
        return 0;
    }

    for (int i = 5; i < strlen(input) - 1; i++)
    {
        if (!isdigit(input[i]))
        {
            return 0;
        }
    }

    return 1;
}

void process(char *input)
{
    if (strncmp(input, "create", 6) == 0)
    {
        create();
        printf("New stack is initialized\n");
        return;
    }
    if (strncmp(input, "empty", 5) == 0)
    {
        if (!initialized())
        {
            printf(ANSI_COLOR_RED "Stack is not initialized!\n");
            return;
        }
        if (empty())
        {
            printf("True\n");
        }
        else
        {
            printf("False\n");
        }

        return;
    }
    if (strncmp(input, "stack_size", 10) == 0)
    {
        if (!initialized())
        {
            printf(ANSI_COLOR_RED "Stack is not initialized!\n");
            return;
        }

        stack_size();

        return;
    }
    if (strncmp(input, "pop", 3) == 0)
    {
        if (!initialized())
        {
            printf(ANSI_COLOR_RED "Stack is not initialized!\n");
            return;
        }
        if (!empty())
        {
            printf("Result: %d\n", pop());
        }
        else
        {
            printf(ANSI_COLOR_RED "Stack is empty\n" ANSI_COLOR_MAGENTA);
        }

        return;
    }
    if (strncmp(input, "peek", 4) == 0)
    {
        if (!initialized())
        {
            printf(ANSI_COLOR_RED "Stack is not initialized!\n");
            return;
        }
        if (!empty())
        {
            printf("Result: %d\n", peek());
        }
        else
        {
            printf(ANSI_COLOR_RED "Stack is empty\n" ANSI_COLOR_MAGENTA);
        }

        return;
    }
    if (strncmp(input, "push", 4) == 0)
    {
        if (!initialized())
        {
            printf(ANSI_COLOR_RED "Stack is not initialized!\n");
            return;
        }
        if(!validate_push(input))
        {
            printf(ANSI_COLOR_RED "Incorrect format!\n");
            return;
        }

        int data;
        sscanf(input, "push(%d)", &data);
        push(data);
        printf("Pushed %d onto the stack\n", data);

        return;
    }
    if (strncmp(input, "display", 7) == 0)
    {
        if (!initialized())
        {
            printf(ANSI_COLOR_RED "Stack is not initialized!\n");
            return;
        }

        if (empty())
        {
            printf("Stack is empty, nothing to display\n");
        }
        else
        {
            display();
        }

        return;
    }
    if (strncmp(input, "help", 4) == 0)
    {
        start_msg();

        return;
    }
    if (strncmp(input, "exit", 4) == 0)
    {
        printf(ANSI_COLOR_RED);
        terminate();

        return;
    }

    printf(ANSI_COLOR_RED "There is no such command. For the list of commands, type \'help\'\n");
}

void client()
{

    printf(ANSI_COLOR_GREEN "\nInput next command: " ANSI_COLOR_RESET);

    char input[32];
    scanf("%s", input);
    getchar();
    printf(ANSI_COLOR_GREEN "\nReceived input: " ANSI_COLOR_RESET);
    printf("%s\n", input);

    write(pipe_fd[1], input, strlen(input));
    kill(server_id, SIGUSR1);
}

void server()
{
    char *input = malloc(32);

    read(pipe_fd[0], input, 32);
    printf(ANSI_COLOR_MAGENTA "Input from the pipe: " ANSI_COLOR_RESET);
    printf("%s\n" ANSI_COLOR_MAGENTA, input);

    process(input);

    kill(getppid(), SIGUSR2);
}

int main()
{
    pipe(pipe_fd);

    start_msg();
    server_id = fork();
    if (server_id == 0)
    {
        // Child process
        signal(SIGUSR1, server);
        while (1)
            ;
    }
    else
    {
        // Parent process
        client();
        signal(SIGUSR2, client);
        while (1)
            ;
    }

    return 0;
}