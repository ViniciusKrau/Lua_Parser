#ifndef _NODES_H
#define _NODES_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Lexer.c"

typedef enum{
    ID,
    STRING,
    RESERVED,
    NUM,
    ASSIGN,
    EXPR,
    OPE
}type;
typedef struct treenode{
    type type;
    char name[MAXTOKENLEN+1];
    treeNode* raiz;
    treeNode* leftnode;
    treeNode* rightnode;
    float numero;
    TokenType token;
    
}treeNode;

#endif