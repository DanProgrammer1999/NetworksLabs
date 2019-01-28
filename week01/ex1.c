#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define MESSAGE_1 "To control the stack you can use following commands:\n\n"
#define MESSAGE_2 "1) create - create a new stack \n2) empty - check is the stack is empty \n"
#define MESSAGE_3 "3) pop \n4) peek \n5) push <number> \n6) display - output all elements in the stack \n"
#define MESSAGE_4 "7) list - print this message \n8) exit - leave the program\n"

int pipe_fd[2];
int pending_push = 0;

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

void client(int server_id)
{
    while (1)
    {
        printf("Input next command: \n");

        char input[32];
        scanf("%s", input);
        printf("Client: received input: %s\n", input);

        write(pipe_fd[1], input, strlen(input));
        kill(server_id, SIGUSR1);
    }
}

void server()
{
    char *input = malloc(32);

    read(pipe_fd[0], input, 32);    

    printf("Server: input from the pipe: %s\n", input);

    if (strcmp(input, "exit") == 0)
    {
        terminate();
    }

    char *command = malloc(8);

}

int main()
{
    pipe(pipe_fd);

    printf("%s%s%s%s\n", MESSAGE_1, MESSAGE_2, MESSAGE_3, MESSAGE_4);

    int fid = fork();
    if (fid == 0)
    {
        // Child process
        printf("This is child\n");
        signal(SIGUSR1, server);
        while (1);
    }
    else
    {
        // Parent process
        printf("This is parent\n");
        client(fid);
    }

    return 0;
}