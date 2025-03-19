#ifndef AST_H_
#define AST_H_

typedef struct NODE
{
    char value;
    struct NODE *left;
    struct NODE *right;

} NODE;

typedef struct
{
    char array[100]; // array to hold elements of the stack
    int top;         // index of the top element
} STACK;

// functions for the Stack
void initializeStack(STACK *stack);
int isEmpty(STACK *stack);
int isFull(STACK *stack);
void push(STACK *stack, char value);
char pop(STACK *stack);
char peek(STACK *stack);

// functions for the AST
NODE *createNode(char value);
void insert(NODE **root, char value);

#endif
