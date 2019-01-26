#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 10

static int *stack;
int head;
int arr_size;

void create()
{
    stack = malloc(sizeof(int) * INIT_SIZE);
    head = -1;
    arr_size = INIT_SIZE;
}

int empty()
{
    return head < 0;
}

void stack_size()
{
    printf("Stack size is %i.\n", head + 1);
}

void grow()
{
    int new_size = arr_size * 2;
    int *new_stack = malloc(sizeof(int) * new_size);
    for (int i = 0; i <= head; i++)
    {
        new_stack[i] = stack[i];
    }
    stack = new_stack;
    arr_size = new_size;
}

int peek()
{
    if (!empty())
    {
        return stack[head];
    }
    else
    {
        printf("Stack is empty.\n");
        return -1;
    }
}

void push(int data)
{
    if (head + 2 >= arr_size)
    {
        grow();
    }

    head++;
    stack[head] = data;
}

int pop()
{
    if (empty())
    {
        perror("Fatal error: cannot pop on empty stack.\n");
        exit(-1);
    }

    return stack[head--];
}

void display()
{
    for (int i = 0; i < head + 1; i++)
    {
        printf("%d ", stack[i]);
    }
    printf("\n");
}
