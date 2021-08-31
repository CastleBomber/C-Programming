#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "parser.h"
#define NAN_RET_VAL (RET_VAL){DOUBLE_TYPE, NAN}
#define ZERO_RET_VAL (RET_VAL){INT_TYPE, 0}
#define ZERO_DOUBLE_RET_VAL (RET_VAL){DOUBLE_TYPE, 0.0}
#define ONE_RET_VAL (RET_VAL){INT_TYPE, 1}
#define BISON_FLEX_LOG_PATH "../src/bison-flex-output/bison_flex_log"

FILE* read_target;
FILE* flex_bison_log_file;

int yyparse(void);
int yylex(void);
void yyerror(char *, ...);
void warning(char*, ...);

typedef enum func_type {
    NEG_FUNC,
    ABS_FUNC,
    ADD_FUNC,
    SUB_FUNC,
    MULT_FUNC,
    DIV_FUNC,
    REMAINDER_FUNC,
    EXP_FUNC,
    EXP2_FUNC,
    POW_FUNC,
    LOG_FUNC,
    SQRT_FUNC,
    CBRT_FUNC,
    HYPOT_FUNC,
    MAX_FUNC,
    MIN_FUNC,
    RAND_FUNC,
    READ_FUNC,
    EQUAL_FUNC,
    LESS_FUNC,
    GREATER_FUNC,
    PRINT_FUNC,
    CUSTOM_FUNC
} FUNC_TYPE;

FUNC_TYPE resolveFunc(char *);

typedef enum num_type {
    INT_TYPE,
    DOUBLE_TYPE,
    NO_TYPE
} NUM_TYPE;

NUM_TYPE resolveType(char *);

typedef struct {
    NUM_TYPE type;
    double value;
} AST_NUMBER;

typedef AST_NUMBER RET_VAL;

typedef struct ast_function {
    FUNC_TYPE func;
    //ident
    struct ast_node *opList;
} AST_FUNCTION;

typedef enum ast_node_type {
    NUM_NODE_TYPE,
    FUNC_NODE_TYPE,
    SYM_NODE_TYPE,
    SCOPE_NODE_TYPE,
    CONDITIONAL_NODE_TYPE
} AST_NODE_TYPE;

typedef struct {
    char* id;
} AST_SYMBOL;

typedef struct {
    struct ast_node *child;
} AST_SCOPE;

typedef struct {
    struct ast_node *condition;
    struct ast_node *trueValue;
    struct ast_node *falseValue;
} AST_CONDITIONAL;

typedef struct ast_node {
    AST_NODE_TYPE type;
    struct ast_node *parent;
    struct symbol_table_node *variableTable;
    struct symbol_table_node *argumentTable;
    union {
        AST_NUMBER number;
        AST_FUNCTION function;
        AST_SYMBOL symbol;
        AST_SCOPE scope;
        AST_CONDITIONAL conditional;
    } data;
    struct ast_node *next;
} AST_NODE;

typedef enum {
    VAR_TYPE,
    LAMBDA_TYPE,
    ARG_TYPE
} SYMBOL_TYPE;

typedef struct stack_node {
    RET_VAL value;
    struct stack_node *next;
};

typedef struct symbol_table_node {
    char *id;
    AST_NODE *value;
    SYMBOL_TYPE symbolType;
    NUM_TYPE numType;
    struct stack_node *stack;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

AST_NODE *createNumberNode(double value, NUM_TYPE type);
AST_NODE *createFunctionNode(FUNC_TYPE func, AST_NODE *opList);
AST_NODE *addExpressionToList(AST_NODE *newExpr, AST_NODE *exprList);

AST_NODE *createScopeNode(SYMBOL_TABLE_NODE *letSection, AST_NODE *child);
SYMBOL_TABLE_NODE *createSymbolTableNode(char *id, AST_NODE *node);
SYMBOL_TABLE_NODE *createSymbolTableNodeWithType(char *id, AST_NODE *node, NUM_TYPE type);
AST_NODE *createSymbolASTNode(char *id);
AST_NODE *createConditionNode(AST_NODE *trueOrFalseCondition, AST_NODE *trueValueNode, AST_NODE *falseValueNode);
AST_CONDITIONAL *createASTConditional(AST_NODE *conditionNode, AST_NODE *trueOrFalseCondition, AST_NODE *trueValueNode, AST_NODE *falseValueNode);

SYMBOL_TABLE_NODE *addLetToList(SYMBOL_TABLE_NODE *newLet, SYMBOL_TABLE_NODE *letList);
RET_VAL typeCastCheck(RET_VAL retVal,SYMBOL_TABLE_NODE *curSymbolTable);
void *checkSymbolTableValueTypeHasNumNodeType(SYMBOL_TABLE_NODE *curSymbolTable, RET_VAL retVal);

RET_VAL eval(AST_NODE *node);
RET_VAL evalConditionNode(AST_NODE *node);
RET_VAL evalLessFunc(AST_NODE *opList);

void printRetVal(RET_VAL val);
void freeNode(AST_NODE *node);
void freeSymbolTableNode(SYMBOL_TABLE_NODE *symbolTableNode);
void freeFunctionNode(AST_NODE *node);
void freeSymbolNode(AST_NODE *node);
void freeScopeNode(AST_NODE *node);
void freeConditionNode(AST_NODE *node);

#endif
