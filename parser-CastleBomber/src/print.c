#include "print.h"

void printProgram(NODE *node, int indent)
{
    if (node == NULL){
        return;
    }

    if (node->leftChild != NULL){
        printStatement(node->leftChild, indent);
    }

    if (node->rightChild != NULL){
        printProgram(node->rightChild, indent);
    }
}

void printStatement(NODE *node, int indent)
{
    printfIndented(indent, "START STATEMENT");

    if (node->leftChild != NULL){
        switch (node->leftChild->type){
            case ASSIGN_STMT_NODE:
                printAssignStmt(node->leftChild, indent + 1);
                break;
            case REPEAT_STMT_NODE:
                printRepeatStmt(node->leftChild, indent + 1);
                break;
            case PRINT_STMT_NODE:
                printPrintStmt(node->leftChild, indent + 1);
                break;
            default:
                error("Child of statement node has invalid type %d, cannot print.", node->leftChild->type);
        }
    }

    printfIndented(indent, "END STATEMENT");
}

void printAssignStmt(NODE *node, int indent)
{
    printfIndented(indent, "START ASSIGN STMT");

    if(node->leftChild->type != IDENT_NODE){
        error("Left child of assign statement is invalid (should be ident node).");
    }
    printId(node->leftChild, indent+1);

    if(node->rightChild->type != EXPR_NODE){
        error("Right child of assign statement is invalid (should be expr node).");
    }
    printExpr(node->rightChild, indent+1);

    printfIndented(indent, "END ASSIGN STMT");

}

void printRepeatStmt(NODE *node, int indent)
{
    printfIndented(indent, "START REPEAT STATEMENT");

    if(node->leftChild->type != EXPR_NODE){
        error("no good");
    }

    printExpr(node->leftChild, indent+1);

    if(node->rightChild->type != STATEMENT_NODE){
        error("no good");
    }

    printStatement(node->rightChild, indent+1);
    printfIndented(indent, "END REPEAT STATEMENT");
}

void printPrintStmt(NODE *node, int indent)
{
    printfIndented(indent, "START PRINT STATEMENT");

    if(node->rightChild->type != EXPR_NODE){
        error("no good");
    }
    printExpr(node->rightChild, indent+1); //?

    printfIndented(indent, "END PRINT STATEMENT");
}

void printExpr(NODE *node, int indent)
{
    printfIndented(indent, "START EXPRESSION");

    if(node->leftChild->type != TERM_NODE){
        error("no good");
    }
    printTerm(node->leftChild, indent+1);

    if (node->rightChild != NULL) {
        printfIndented(indent, "<ADDOP %c>", node->data.op);
        printExpr(node->rightChild, indent + 1);
    }

    printfIndented(indent, "END EXPRESSION");
}

void printTerm(NODE *node, int indent)
{
    printfIndented(indent, "START TERM");

    if(node->leftChild->type != FACTOR_NODE){
        error("no good");
    }

    printFactor(node->leftChild, indent+1);

    printfIndented(indent, "END TERM");
}

void printFactor(NODE *node, int indent)
{
    printfIndented(indent, "START FACTOR");

    switch (node->leftChild->type) {
        case IDENT_NODE:
            printId(node->leftChild, indent+1);
            break;
        case NUMBER_NODE:
            printNumber(node->leftChild, indent+1);
            break;
        case FACTOR_NODE:
            if(node->data.op != NULL){
                printfIndented(indent+1, "<ADDOP %c>", node->data.op);
            }
            printFactor(node->leftChild, indent+1);
            break;
        case EXPR_NODE:
            break;
    }

    printfIndented(indent, "END FACTOR");
}

void printId(NODE *node, int indent)
{
    printfIndented(indent, "START IDENT");
    printfIndented(indent+1, "%s", node->data.identifier);
    printfIndented(indent, "END IDENT");
}

void printNumber(NODE *node, int indent)
{
    printfIndented(indent, "START NUMBER");

    switch(node->data.number.type){
        case INT_TYPE:
            printfIndented(indent+1, "INT : %d", node->data.number.value.integral);
            break;
        case FLOAT_TYPE:
            printfIndented(indent+1, "FLOAT : %f",node->data.number.value.floating_point);
            break;
    }

    printfIndented(indent, "END NUMBER");
}

void printfIndented(int indent, char *format, ...)
{
    printf("\n");

    for (int i = 0; i < indent; ++i){
        printf("|  ");
    }

    char buffer[256];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, 255, format, args);
    printf("%s", buffer);
    va_end (args);
}

