/**
 * [COMP 232] Scanner
 *
 * Created by Bieszczad, A.J. on 9/20/17.
 * Copyright (c) 2017 CSUCI. All rights reserved.
 *
 * Author: CastleBomber
 * Date: May 3, 2021
 */

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFFER_SIZE 32

// clean up the referenced token (if it isn't NULL).
void freeToken(TOKEN **token)
{
    /*
     * TODO
     * free the referenced TOKEN*, and any data in
     * the referenced TOKEN which requires freeing
     * (not in that order).
     *
     * Then, set the referenced TOKEN* to NULL.
     */
    //free((*token)->val.string);
    free(*token);

    *token = NULL;
}

void printToken (TOKEN **token)
{
    /*
     * TODO
     * Print the referenced token in a readable format.
     * Displayed information should include the token's type,
     * and also the token's value if applicable.
     *
     * Check the sample runs for an example format.
     */


    switch ((*token)->type) {
        case NO_TOKEN_TYPE:
            break;
        case INVALID_TOKEN:
            printf("<INVALID, %s>\n", (*token)->val.string);
            //printf("<INVALID, %c>\n", (*token)->val.op);
            break;
        case EOF_TOKEN:
            printf("<EOF>\n");
            break;
        case INT_TOKEN:
            printf("<INT, %s>\n", (*token)->val.string);
            //printf("<INT, %ld>\n", (*token)->val.integer);
            break;
        case FLOAT_TOKEN:
            //printf("<FLOAT, %s>\n", (*token)->val.string);
            (*token)->val.floating_point = strtod((*token)->val.string, NULL);
            printf("<FLOAT, %f>\n", (*token)->val.floating_point);
            break;
        case ADDOP_TOKEN:
            printf("<ADD_OP, %s>\n", (*token)->val.string);
            break;
        case MULTOP_TOKEN:
            printf("<MULT_OP, %s>\n", (*token)->val.string);
            break;
        case IDENT_TOKEN:
            //printf("token type as int %d\n", (*token)->type);
            printf("<IDENT, %s>\n", (*token)->val.string);
            break;
        case PRINT_TOKEN:
            printf("<KEYWORD, print>\n");
            break;
        case REPEAT_TOKEN:
            printf("<KEYWORD, repeat>\n");
            break;
        case SEMICOLON_TOKEN:
            printf("<SEMICOLON>\n");
            break;
        case LPAREN_TOKEN:
            printf("<LPAREN>\n");
            break;
        case RPAREN_TOKEN:
            printf("<RPAREN>\n");
            break;
        case ASSIGN_TOKEN:
            printf("<ASSIGN>\n");
            break;
    }
    //sleep(2);
}

void checkKeywordOrID(TOKEN *token, char *str)
{
    /*
     * TODO
     * For use in the scanner function, to differentiate between keywords
     * and identifiers (which are treated identically by the state machine).
     *
     * Check if the collected token is a keyword by comparing the string
     * value arg to the string values of the keywords (print and repeat).
     * If the string value denotes is a print or repeat token, change its
     * type to the corresponding TOKEN_TYPE.
     *
     * If the token is not a keyword, then it is an identifier.
     * Change its type accordingly, and store its string value.
     * Note that the str argument is the scanner's buffer,
     * so it should not be assigned directly, but copied;
     * the buffer will be freed before the token
     * is returned (see the bottom of the scanner function).
     *
     * Recall, to copy a string:
     * 1. Allocate enough space to store the string (including the null terminator).
     * 2. Copy the characters into this new space from the original.
     */

    size_t bufferSize = BUFFER_SIZE;
    char *cpyStr = calloc(sizeof(char), bufferSize);

    if((strcmp(str,"print") == 0)){
        token->type = PRINT_TOKEN;
    } else if((strcmp(str,"repeat") == 0)){
        token->type = REPEAT_TOKEN;
    } else{
        strcpy(cpyStr, str);
        token->val.string = cpyStr;
    }
}

/*
    stringValue is a buffer to store a token's contained characters
    while it is being lexed. You should add characters to it like an
    array, and be sure to keep the stringValueIndex up to date so you
    know where the next character should go.

    Dont forget to terminate string values when they're complete!
*/
TOKEN *scanner()
{
    size_t bufferSize = BUFFER_SIZE;
    char *stringValue = calloc(sizeof(char), bufferSize);
    int stringValueIndex = 0;

    TOKEN *token = (TOKEN *) malloc(sizeof(TOKEN)); // initialize the token type to denote that the token is in progress
    token->type = NO_TOKEN_TYPE; // initialize the token type to denote that the token is in progress
    FSM_STATE state = START_STATE; // set state machine to starting state

    char currentChar = '\0';

    while (currentChar != EOF && token->type == NO_TOKEN_TYPE){
        currentChar = (char) getchar();

        switch(state){
            case START_STATE:
                switch(currentChar){
                    case ';':
                        token->type = SEMICOLON_TOKEN;
                        break;
                    case '0'...'9':
                        stringValue[stringValueIndex++] = currentChar;
                        state = INT_STATE;
                        break;
                    case '+':
                    case '-':
                        stringValue[stringValueIndex] = currentChar;
                        token->type = ADDOP_TOKEN;
                        break;
                    case '*':
                    case '/':
                    case '%':
                        stringValue[stringValueIndex] = currentChar;
                        token->type = MULTOP_TOKEN;
                        break;
                    case 'a'...'z':
                    case 'A'...'Z':
                    case '$':
                    case '_':
                        stringValue[stringValueIndex++] = currentChar;
                        state = IDENT_OR_KEYWORD_STATE;
                        break;
                    case '(':
                        token->type = LPAREN_TOKEN;
                        break;
                    case ')':
                        token->type = RPAREN_TOKEN;
                        break;
                    case '=':
                        token->type = ASSIGN_TOKEN;
                    case ' ':
                    case '\n':
                    case '\t':
                    case '\r':
                        break;
                    case EOF:
                        token->type = EOF_TOKEN;
                        break;
                    default:
                        stringValue[stringValueIndex++] = currentChar;
                        token->type = INVALID_TOKEN;
                        //token->val.op = currentChar;
                        break;
                }
                break;
            case INT_STATE:
                switch (currentChar) {
                    case '0'...'9':
                        stringValue[stringValueIndex++] = currentChar;
                        break;
                    case '.':
                        state = FLOAT_STATE;
                        stringValue[stringValueIndex++] = currentChar;
                        break;
                    default:
                        token->type = INT_TOKEN;
                        ungetc(currentChar, stdin);
                        break;
                }
                break;//
            case FLOAT_STATE:
                switch (currentChar) {
                    case '0'...'9':
                        stringValue[stringValueIndex++] = currentChar;
                        break;
                    default:
                        token->type = FLOAT_TOKEN;
                        //token->val.floating_point = strtod(stringValue, NULL); // being overwritten somehow
                        ungetc(currentChar,stdin);
                        break;
                }
                break;
            case IDENT_OR_KEYWORD_STATE:
                switch(currentChar){
                    case 'a'...'z':
                    case 'A'...'Z':
                    case '0'...'9':
                    case '$':
                    case '_':
                        stringValue[stringValueIndex++] = currentChar;
                        break;
                    default:
                        token->type = IDENT_TOKEN;
                        ungetc(currentChar, stdin);
                        break;
                }
                break;
        }

        /*
         * TODO
         * Given the current state of the state machine
         * and the next character, update the state machine
         * (and the string value of the token being built, if applicable).
         *
         * DO NOT return anything here. The only return is the
         * one at the end of this function. Note that the while
         * loop stops iterating once token->type is updated, so
         * token->type should only be updated once the token is
         * ready to be returned.
         */

        checkKeywordOrID(token, stringValue);

        // if the buffer is full, quadruple its size (do not edit)
        if (stringValueIndex >= bufferSize-1)
        {
            bufferSize *= 4;
            char *temp = calloc(sizeof(char), bufferSize);
            strcpy(temp, stringValue);
            free(stringValue);
            stringValue = temp;
        }
    }
    free(stringValue);
    return token;
}

