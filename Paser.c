#include <stdio.h>

#include "Lexer.c"

typedef struct treenode{
        TokenType token;
        struct node *leftnode;
        struct node *rightnode;
    }node;

void createnode(){
    
}

void main(){


    char filename[15];
    printf("Digite o nome do arquivo com extensao");
    scanf("%s", &filename);
    FILE* source = fopen("", "r");
    getToken(source);

}