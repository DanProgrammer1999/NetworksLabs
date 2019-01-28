#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define INIT_SIZE 20

static int *stack;
int head;
int arr_size;
int init = 0;

void create()
{
    init = 1;
    stack = malloc(sizeof(int) * INIT_SIZE);
    head = -1;
    arr_size = INIT_SIZE;
}

int initialized()
{
    return init;
}

int empty()
{
    if (!initialized())
    {
        return EOF;
    }
    return head < 0;
}

void stack_size()
{
    if (!initialized())
    {
        return;
    }

    printf("Stack size is %i\n", head + 1);
}

void grow()
{
    int new_size = arr_size * 2;
    int *new_stack = malloc(sizeof(int) * new_size);
    for (int i = 0; i <= head; i++)
    {
        new_stack[i] = stack[i];
    }
    free(stack);
    stack = new_stack;
    arr_size = new_size;
}

int peek()
{
    if (!initialized())
    {
        return EOF;
    }
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
    if (!initialized())
    {
        return;
    }
    if (head + 2 >= arr_size)
    {
        grow();
    }

    head++;
    stack[head] = data;
}

int pop()
{
    if (!initialized())
    {
        return EOF;
    }

    if (empty())
    {
        perror("Fatal error: cannot pop on empty stack.\n");
        exit(-1);
    }

    return stack[head--];
}

void display()
{
    if (!initialized())
    {
        return;
    }

    for (int i = 0; i < head + 1; i++)
    {
        printf("%d ", stack[i]);
    }
    printf("\n");
}