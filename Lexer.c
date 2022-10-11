#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define BUFLEN 256
#define MAXRESERVED 22
#define MAXTOKENLEN 40

typedef enum{
    true = 1,
    false = 0
}bType;

typedef enum {
    /* tokens de controle */
    ENDFILE,
    ERROR,
    /* palavras reservadas */
    AND,
    BREAK,
    DO,
    ELSE,
    ELSEIF,
    END,
    FALSE,
    FOR,
    FUNCTION,
    GOTO,
    IF,
    IN,
    LOCAL,
    NIL,
    NOT,
    OR,
    REPEAT,
    RETURN,
    THEN,
    TRUE,
    UNTIL,
    WHILE,
    /* tokens multicaractere */
    ID,
    NUM,
    STRING,
    /* simbolos especiais */
    PLUS,
    MINUS,
    TIMES,
    OVER,
    REMAIN,
    POWER,
    COMMENT,
    FOVER,
    EQ,
    NEQ,
    GTEQ,
    LTEQ,
    GT,
    LT,
    ASSIGN,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    LCHAVE,
    RCHAVE,
    MCALL,
    SEMI,
    CONCAT,
    DOT,
    COMMA,
} TokenType;

typedef enum {
    START,
    INASSIGN,
    INCOMMENTL,
    INCOMMENTB,
    INNUM,
    INID,
    INCOMPARE,
    INSTRING,
    DONE
} StateType;

struct {
    char *str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"and", AND},     {"break", BREAK},   {"do", DO},
    {"else", ELSE},   {"elseif", ELSEIF}, {"end", END},
    {"false", FALSE}, {"for", FOR},       {"function", FUNCTION},
    {"goto", GOTO},   {"if", IF},         {"in", IN},
    {"local", LOCAL}, {"nil", NIL},       {"not", NOT},
    {"or", OR},       {"repeat", REPEAT}, {"return", RETURN},
    {"then", THEN},   {"true", TRUE},     {"until", UNTIL},
    {"while", WHILE}};

bType EOF_flag = false;  // Confirma se fgets conseguiu ler uma linha nova.
char tokenString[MAXTOKENLEN+1];
int tokenStringIndex = 0;
char lineBuf[BUFLEN];        // linha atual
int lineno = 0, linepos = 0, /* posicao em lineBuf */ bufsize = 0; /* tamanho atual da string no buffer */

TokenType reservedLookup(char *s) {
    int i;
    for (i = 0; i < MAXRESERVED; i++)
        if (!strcmp(s, reservedWords[i].str)) return reservedWords[i].tok;
    return ID;
}


int totalchar(){
    int maxchar = 0;
    while(fgetc(source)) maxchar++;
    rewind(source);
}

int getNextChar() { // Le uma linha e retorna o proximo caracter
    if (linepos >= bufsize) {  // Verifica se ja leu toda a linha guardada em linebuf
        lineno++;
        if (fgets(lineBuf, (BUFLEN - 1), source)) {  // Le uma linha do codigo fonte e guarda em linebuf
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        } else {
            EOF_flag = true;  // verifica se o arquivo acabou.
            return EOF;
        }
    }
    return lineBuf[linepos++];
}

int ungetNextChar() {
    if (!EOF_flag) linepos--;
}

TokenType getToken() {
    int c;
    bType save = true;
    TokenType currentToken;  // Token a ser retornado
    char identificador[20];
    StateType state = START;  // Estado definido para começar
    while (state != DONE) {
        c = getNextChar(source);
        save = true;
        switch (state) {
            case START: {
                if (isdigit(c)) state = INNUM;
                else if (isalpha(c)) state = INID;
                else if(c == '"') state = INSTRING;
                else if (c == '=') {
                    if (getNextChar(source) == '=') {
                        currentToken = EQ;
                        state = DONE;
                        save = false;
                    } else {
                        ungetNextChar();
                        state = INASSIGN;
                    }
                }
                else if ((c == ' ') || (c == '\t')) save = false;
                else if (c == '\n') {
                    save = false;
                }
                else if (c == '-') {
                    if (getNextChar(source) == '-') {
                        save = false;
                        state = INCOMMENTL;
                        if (getNextChar(source) == '[') {
                            if (getNextChar(source) == '[') {
                                state = INCOMMENTB;
                            } else {
                                ungetNextChar();
                                ungetNextChar();
                            }
                        } 
                        else ungetNextChar();
                    } 
                    else {
                        currentToken = MINUS;
                        state = DONE;
                    }
                } 
                else {
                    state = DONE;
                    switch (c) {
                        case EOF:
                            save = false;
                            currentToken = ENDFILE;
                            break;
                        case '<':
                            if (getNextChar(source) == '=') {
                                currentToken = LTEQ;
                            } 
                            else {
                                currentToken = LT;
                                ungetNextChar();
                            }
                            break;
                        case '>':
                            if (getNextChar(source) == '=') {
                                currentToken = GTEQ;
                            } 
                            else {
                               currentToken = GT;
                                ungetNextChar();
                            }
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case '/':
                            if (getNextChar(source) == '/') {
                                currentToken = FOVER;
                            } 
                            else {
                                currentToken = OVER;
                                ungetNextChar();
                            }
                            break;
                        case '(':
                            currentToken = LPARENT;
                            break;
                        case ')':
                            currentToken = RPARENT;
                            break;
                        case '{':
                            currentToken = LCHAVE;
                            break;
                        case '}':
                            currentToken = RCHAVE;
                            break;
                        case '[':
                            currentToken = LBRACK;
                            break;
                        case ']':
                            currentToken = RBRACK;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        case '%':
                            currentToken = REMAIN;
                            break;
                        case '^':
                            currentToken = POWER;
                            break;
                        case '&':
                            currentToken = AND;
                            break;
                        case '|':
                            currentToken = OR;
                            break;
                        case '~':
                            if (getNextChar(source) == '=')
                                currentToken = NEQ;
                            else
                                ungetNextChar();
                            break;
                        case '.':
                            if (getNextChar(source) == '.')
                                currentToken = CONCAT;
                            else {
                                ungetNextChar();
                                currentToken = DOT;
                            }
                            break;
                        case ',':
                            currentToken = COMMA;
                            break;
                        case ':':
                            currentToken = MCALL;  // METHOD CALL
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;
            }
            case INSTRING: 
                if (c == '"') {
                    state = DONE;
                    currentToken = STRING;
                }
                break;
            case INCOMMENTL:  // Trata comentario linha '--'
                save = false;
                if (c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                } else if (c == '\n') {
                    state = DONE;
                }
                currentToken = COMMENT;
                break;
            case INCOMMENTB:  // Trata comentario em bloco --[[  --]]
                save = false;
                if (c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                } else if (c == '-') {
                    if (getNextChar(source) == '-') {
                        if (getNextChar(source) == ']') {
                            if (getNextChar(source) == ']') {
                                state = DONE;
                            }
                        } else {
                            ungetNextChar();
                            ungetNextChar();
                        }
                    } else
                        ungetNextChar();
                }
                currentToken = COMMENT;
                break;
            case INASSIGN:
                state = DONE;
                currentToken = ASSIGN;
                break;
            case INNUM:
                if (!isdigit(c) && c != '.') {
                    ungetNextChar();
                    save = false;
                    state = DONE;
                    currentToken = NUM;
                }
                break;
            case INID:
                if (!isalpha(c)) {
                    ungetNextChar();
                    save = false;
                    state = DONE;
                    currentToken = ID;
                }
                break;
            default: /* nao deve acontecer */
                printf("Scanner Bug: state= %d\n", state);
                state = DONE;
                currentToken = ERROR;
                break;
        }  // Switch (state)

    if (save &&  (tokenStringIndex <= MAXTOKENLEN)) tokenString[tokenStringIndex++] = (char)c;
    if (state == DONE) {
        tokenString[tokenStringIndex] = '\0';
        if (currentToken == ID) currentToken = reservedLookup(tokenString);
    }
    }  // While state != DONE
    char tokenString[MAXTOKENLEN-1];
    tokenStringIndex = 0;
    return currentToken; // retorna o token definido
}  // GetToken

// void main(){
//     char teste[30];
//     FILE *source = fopen("lua.txt", "r+");
//     FILE *output = fopen("output.txt", "w+");
//     while (getToken(source) != ENDFILE){    }
//     fclose(source);
//     fclose(output);
// }