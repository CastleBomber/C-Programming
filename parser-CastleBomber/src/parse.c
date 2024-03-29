#include "parse.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

TOKEN *getNextToken(TOKEN **token)
{
    freeToken(token);
    TOKEN* tok = scanner();
    //printToken(&tok); fflush(stdout); // uncomment the line below if desired for debugging purposes.

    return tok;
}

NODE *program()
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = PROGRAM_NODE;
    node->leftChild = statement();

    if (node->leftChild != NULL){
        node->rightChild = program();
    }
    else{
        free(node);
        node = NULL;
    }
    return node;
}

NODE *statement()
{
    TOKEN *token = getNextToken(NULL);
    if (token == NULL){
        return NULL;
    }
    else if (token->type == EOF_TOKEN){
        freeToken(&token);
        return NULL;
    }

    NODE *node = calloc(sizeof(NODE), 1);
    node->type = STATEMENT_NODE;

    switch(token->type){
        case IDENT_TOKEN:
            node->leftChild = assignStmt(&token);
            break;
        case REPEAT_TOKEN:
            node->leftChild = repeatStmt(&token);
            break;
        case PRINT_TOKEN:
            node->leftChild = printStmt(&token);
            break;
        default:
            // see the TOKEN_TYPE enum to see which numbers mean what...
            error("Invalid token at start of statement.");
    }

    return node;
}

NODE *assignStmt(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = ASSIGN_STMT_NODE;
    node->leftChild = ident(currToken);
    *currToken = getNextToken(currToken); // should grab '='

    if((*currToken)->type != ASSIGNMENT_TOKEN){
        error("Invalid token in assignment statement; expected assignment token.");
    }

    *currToken = getNextToken(currToken);
    node->rightChild = expr(currToken); // currtoken 100
    *currToken = getNextToken(currToken); // should grab ';'

    if((*currToken)->type != SEMICOLON_TOKEN){
        error("Invalid token is assignment; expected semicolon token.");
    }

    freeToken(currToken);

    return node;
}

NODE *repeatStmt(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = REPEAT_STMT_NODE;
    *currToken = getNextToken(currToken); // should grab '('

    if((*currToken)->type != LPAREN_TOKEN){
        error("Invalid token in repeat statement; expected (.");
    }

    *currToken = getNextToken(currToken); // should be expr
    node->leftChild = expr(currToken);
    *currToken = getNextToken(currToken); // should grab ')'

    if((*currToken)->type != RPAREN_TOKEN){
        error("Invalid token in repeat statement; expected ).");
    }

    //*currToken = getNextToken(currToken);
    //node->rightChild = statement(currToken);
    node->rightChild = statement();

    return node;
}

NODE *printStmt(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = PRINT_STMT_NODE;
    *currToken = getNextToken(currToken); // should grab '('

    if((*currToken)->type != LPAREN_TOKEN){
        error("no good");
    }

    *currToken = getNextToken(currToken); // should grab exp
    node->rightChild = expr(currToken);
    *currToken = getNextToken(currToken);

    if((*currToken)->type != RPAREN_TOKEN){
        error("no good");
    }

    *currToken = getNextToken(currToken); // should grab ';'

    if((*currToken)->type != SEMICOLON_TOKEN){
        error("Invalid token is assignment; expected semicolon token.");
    }

    freeToken(currToken);

    return node;
}

NODE *expr(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = EXPR_NODE;
    node->leftChild = term(currToken);

    *currToken = getNextToken(currToken);

    if((*currToken)->type == ADD_OP_TOKEN) {
        node->data.op = (*currToken)->val.op;
        *currToken = getNextToken(currToken);
        node->rightChild = expr(currToken);
    } else{
        ungetToken(currToken);
    }

    return node;
}

NODE *term(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = TERM_NODE;
    node->leftChild = factor(currToken);

    return node;
}

NODE *factor(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = FACTOR_NODE;

    switch((*currToken)->type){
        case INT_TOKEN:
        case FLOAT_TOKEN:
            node->leftChild = number(currToken);
            break;
        case IDENT_TOKEN:
            node->leftChild = ident(currToken);
            break;
        case ADD_OP_TOKEN:
            node->data.op = (*currToken)->val.op;
            *currToken = getNextToken(currToken);
            node->leftChild = factor(currToken);
            break;
    }

    return node;
}

NODE *ident(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = IDENT_NODE;
    node->data.identifier =  (*currToken)->val.string;

    (*currToken)->val.string = NULL; // we dynamically allocate space for string value, need to free up
    freeToken(currToken); //redundant

    return node;
}

NODE *number(TOKEN **currToken)
{
    NODE *node = calloc(sizeof(NODE), 1);
    node->type = NUMBER_NODE;

    if((*currToken)->type == INT_TOKEN){
        node->data.number.type = INT_TYPE;
        node->data.number.value.integral = (*currToken)->val.integral;
    } else if((*currToken)->type == FLOAT_TOKEN){
        node->data.number.type = FLOAT_TYPE;
        node->data.number.value.floating_point = (*currToken)->val.floating_point;
    }

    return node;
}

void freeParseTree(NODE **node)
{
    if (node == NULL)
    {
        return;
    }

    if ((*node) == NULL)
    {
        return;
    }

    freeParseTree(&(*node)->leftChild);
    freeParseTree(&(*node)->rightChild);

    if ((*node)->type == IDENT_NODE)
    {
        free((*node)->data.identifier);
    }

    free(*node);
    *node = NULL;
}

void error(char *errorFormat, ...)
{
    char buffer[256];
    va_list args;
    va_start (args, errorFormat);
    vsnprintf (buffer, 255, errorFormat, args);

    printf("\nPARSING ERROR: %s\nExiting...\n", buffer);

    va_end (args);
    exit(1);
}
