#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Nodes.h"
#include "Lexer.c"


FILE* source;

treeNode* root = NULL;

treeNode* createnode(){
    treeNode* temp;
    temp = malloc(sizeof (treeNode));
    temp->leftnode = NULL;
    temp->rightnode = NULL;
    temp->token = NULL;
    temp->raiz = NULL;
    temp->numero = 0;
    return temp;
}

void newnode(TokenType token, TokenType operando){
    treeNode* node = createnode();
    node->leftnode = createnode();
    node->rightnode = createnode();
    if (root == NULL){
        root = node;
    }
    if (token == ID){
        strcpy(node->name, "");
    }
    if (token == NUM){
        node->rightnode;
    }
}

void main(){
    char filename[15];
    printf("Digite o nome do arquivo com extensao");
    scanf("%s", &filename);
    source = fopen(filename, "r");
    getToken(source);

}