#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Nodes.h"
#include "Lexer.c"


FILE* source;

treeNode* root = NULL;
TokenType prevToken;

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

treeNode* newnode(){
    TokenType token = getToken();
    if (token == ENDFILE || ) return;
    treeNode* node = createnode();
    if (root == NULL) root = node;
    if (isID){
        if (isReserved(token)){
            node->token = token;
            node->type = RESERVED;
            node->leftnode = newnode();
        }
        node->type = ID;
        if(prevToken)
        prevToken = ID;
    }
    switch (token){
        case ID:{
            if (isReserved(token)){

            }
            node->type = ID;
            if(prevToken)
            prevToken = ID;
            break;
        }
    }
}

void main(){
    char filename[15];
    printf("Digite o nome do arquivo com extensao");
    scanf("%s", &filename);
    source = fopen(filename, "r");
    getToken(source);

}