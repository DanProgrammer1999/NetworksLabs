#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "stack.h"

#define MESSAGE_1 "To control the stack you can use following commands:\n\n"
#define MESSAGE_2 "create - create a new stack \nempty - check is the stack is empty \n"
#define MESSAGE_3 "pop \npeek \npush(int data) - enter without any spaces \ndisplay - output all elements in the stack \n"
#define MESSAGE_4 "list - print this message \nexit - leave the program\n"

int pipe_fd[2];
int pending_push = 0;
int server_id;

int convert(char *input)
{
    if (strcmp(input, "create"))
    {
        return 1;
    }
    if (strcmp(input, "empty"))
    {
        return 2;
    }
    if (strcmp(input, "pop"))
    {
        return 3;
    }
    if (strcmp(input, "peek"))
    {
        return 4;
    }
    if (strcmp(input, "push"))
    {
        return 5;
    }
    if (strcmp(input, "display"))
    {
        return 6;
    }
    if (strcmp(input, "list"))
    {
        return 7;
    }
    if (strcmp(input, "exit"))
    {
        return 8;
    }
}

void terminate()
{
    printf("Exiting program\n");
    kill(getppid(), SIGKILL);
    exit(0);
}

void client()
{

    printf("Input next command: \n");

    char input[32];
    scanf("%s", input);
    printf("Client: received input: %s\n", input);

    write(pipe_fd[1], input, strlen(input));
    kill(server_id, SIGUSR1);
}

void server()
{
    char *input = malloc(32);

    read(pipe_fd[0], input, 32);    

    printf("Server: input from the pipe: %s\n", input);
    
    if (strncmp(input, "exit", 4) == 0)
    {
        terminate();
    }

    char *command = malloc(8);

    kill(getppid(), SIGUSR2);
}

int main()
{
    pipe(pipe_fd);

    printf("%s%s%s%s\n", MESSAGE_1, MESSAGE_2, MESSAGE_3, MESSAGE_4);

    server_id = fork();
    if (server_id == 0)
    {
        // Child process
        signal(SIGUSR1, server);
        while (1);
    }
    else
    {
        // Parent process
        client();
        signal(SIGUSR2, client);
        while(1);
    }

    return 0;
}