#ifndef AST_H_
#define AST_H_

typedef struct NODE
{
    char* value;    //holds the string value
    struct NODE **children; //dynamic array of pointers to child nodes
    int child_count;    //current num of child nodes
    int capacity;   //max capacity of the child array

} NODE;

typedef struct
{
    char *array[100]; //array to hold elements of the stack
    int top;         //index of the top element
} STACK;

typedef struct
{
    char *non_terminal;
    char *terminal;
    char *production;
} ParsingRule;

// functions for the Stack
void initializeStack(STACK *stack);
int isEmpty(STACK *stack);
int isFull(STACK *stack);
void push(STACK *stack, const char *value);
char* pop(STACK *stack);
char* peek(STACK *stack);

// functions for the AST
NODE *createNode(char* value);
void add_child(NODE *parent, NODE *child);

#endif
