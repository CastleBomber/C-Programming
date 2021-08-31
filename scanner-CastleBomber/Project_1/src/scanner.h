/**
 * [COMP 232] Scanner
 *
 * Created by Bieszczad, A.J. on 9/20/17.
 * Copyright (c) 2017 CSUCI. All rights reserved.
 *
 * Author: CastleBomber
 * Date: May 3, 2021
 */

#ifndef __SCANNER_H
#define __SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    NO_TOKEN_TYPE,
    INVALID_TOKEN,
    EOF_TOKEN,
    INT_TOKEN,
    FLOAT_TOKEN,
    ADDOP_TOKEN,
    MULTOP_TOKEN,
    IDENT_TOKEN,
    PRINT_TOKEN,
    REPEAT_TOKEN,
    SEMICOLON_TOKEN,
    LPAREN_TOKEN,
    RPAREN_TOKEN,
    ASSIGN_TOKEN
} TOKEN_TYPE;

typedef union
{
    long integer;
    double floating_point;
    char *string;
    char op;
} TOKEN_VALUE;

typedef struct token
{
    TOKEN_TYPE type;
    TOKEN_VALUE val;
} TOKEN;

typedef enum
{
    START_STATE,
    IDENT_OR_KEYWORD_STATE,
    INT_STATE,
    FLOAT_STATE
} FSM_STATE;

TOKEN *scanner();
void freeToken(TOKEN **);
void printToken(TOKEN **);

#endif