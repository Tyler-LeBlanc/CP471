#include <stdio.h>

#ifndef AST_H_
#define AST_H_

typedef struct
{
    char *value;
    struct AST_NODE *left;
    struct AST_NODE *right;

}AST_NODE;



#endif
