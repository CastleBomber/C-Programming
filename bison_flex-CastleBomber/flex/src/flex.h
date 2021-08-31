#ifndef __flex_h_
#define __flex_h_

#include <stdio.h>

typedef enum {
    EOF_TOKEN=0,
    IF_TOKEN,
    WHILE_TOKEN,
    REPEAT_TOKEN,
    PRINT_TOKEN,
    INT_TOKEN,
    FLOAT_TOKEN,
    IDENT_TOKEN,
    ADDOP_TOKEN,
    MULTOP_TOKEN,
    LPAREN_TOKEN,
    RPAREN_TOKEN,
    SEMICOLON_TOKEN,
    ASSIGN_TOKEN
} TOKEN;

int yylex(void);
void yyerror(char *s);

#endif
