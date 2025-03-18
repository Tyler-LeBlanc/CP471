#ifndef AST_H_
#define AST_H_

typedef struct
{
    char *value;
    struct NODE *left;
    struct NODE *right;

}NODE;


typedef struct
{
    char array[100];
    int top;
}STACK;


// functions for the Stack
void initializeStack;
int isEmpty;
int isFull;
void push;
char pop;
char peek;


// functions for the AST
NODE* createNode;
void insert;





#endif
