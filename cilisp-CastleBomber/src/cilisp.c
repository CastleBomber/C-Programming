#include "cilisp.h"

#define RED             "\033[31m"
#define RESET_COLOR     "\033[0m"

/*
 yyerror:
 Something went so wrong that the whole program should crash.
 You should basically never call this unless an allocation fails.
 (see the "yyerror("Memory allocation failed!")" calls and do the same.
 This is basically printf, but red, with "\nERROR: " prepended, "\n" appended,
 and an "exit(1);" at the end to crash the program.
 It's called "yyerror" instead of "error" so the parser will use it for errors too.
 */
void yyerror(char *format, ...){
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, 255, format, args);

    printf(RED "\nERROR: %s\nExiting...\n" RESET_COLOR, buffer);
    fflush(stdout);

    va_end (args);
    exit(1);
}

/*
 warning:
 Something went mildly wrong (on the user-input level, probably)
 Let the user know what happened and what you're doing about it.
 Then, move on. No big deal, they can enter more inputs. ¯\_(ツ)_/¯
 You should use this pretty often:
      too many arguments, let them know and ignore the extra
      too few arguments, let them know and return NAN
      invalid arguments, let them know and return NAN
      many more uses to be added as we progress...
 This is basically printf, but red, and with "\nWARNING: " prepended and "\n" appended.
 */
void warning(char *format, ...){
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, 255, format, args);

    printf(RED "WARNING: %s\n" RESET_COLOR, buffer);
    fflush(stdout);

    va_end (args);
}

FUNC_TYPE resolveFunc(char *funcName){
    // Array of string values for function names.
    // Must be in sync with members of the FUNC_TYPE enum in order for resolveFunc to work.
    // For example, funcNames[NEG_FUNC] should be "neg"
    char *funcNames[] = {
            "neg",
            "abs",
            "add",
            "sub",
            "mult",
            "div",
            "remainder",
            "exp",
            "exp2",
            "pow",
            "log",
            "sqrt",
            "cbrt",
            "hypot",
            "max",
            "min",
            "rand",
            "read",
            "equal",
            "less",
            "greater",
            "print",
            ""
    };

    int i = 0;
    while (funcNames[i][0] != '\0'){
        if (strcmp(funcNames[i], funcName) == 0){
            return i;
        }
        i++;
    }
    return CUSTOM_FUNC;
}

NUM_TYPE resolveType(char *typeName){
    char *typeNames[] = {
            "int",
            "double",
            ""
    };

    int i = 0;
    while (typeNames[i][0] != '\0'){
        if (strcmp(typeNames[i], typeName) == 0){
            return i;
        }
        i++;
    }
    return NO_TYPE;
}

AST_NODE *createScopeNode(SYMBOL_TABLE_NODE *letSection, AST_NODE *child){
    AST_NODE *scope;
    size_t nodeSize =  sizeof(AST_NODE);

    if ((scope = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    scope->type = SCOPE_NODE_TYPE;
    scope->data.scope.child = child;
    child->parent = scope;
    child->variableTable = letSection;

    while(letSection){
        letSection->value->parent = child;
        letSection = letSection->next;
    }

    return scope;
}

SYMBOL_TABLE_NODE *createSymbolTableNode(char *id, AST_NODE *node){
    SYMBOL_TABLE_NODE *symbolTableNode;
    size_t nodeSize =  sizeof(SYMBOL_TABLE_NODE);

    if ((symbolTableNode = calloc(nodeSize, 1)) == NULL) {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    symbolTableNode->id = id;
    symbolTableNode->value = node;
    symbolTableNode->numType = NO_TYPE;

    return symbolTableNode;
}

SYMBOL_TABLE_NODE *createSymbolTableNodeWithType(char *id, AST_NODE *node, NUM_TYPE type){
    SYMBOL_TABLE_NODE *symbolTableNode;
    size_t nodeSize =  sizeof(SYMBOL_TABLE_NODE);

    if ((symbolTableNode = calloc(nodeSize, 1)) == NULL) {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    symbolTableNode->id = id;
    symbolTableNode->value = node;
    symbolTableNode->numType = type;

    return symbolTableNode;
}

AST_NODE *createSymbolASTNode(char *id){
    AST_NODE *symbolNode;
    size_t nodeSize=  sizeof(AST_NODE);

    if ((symbolNode = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    symbolNode->type = SYM_NODE_TYPE;
    symbolNode->data.symbol.id = id;

    return symbolNode;
}

AST_NODE *createConditionNode(AST_NODE *trueOrFalseCondition, AST_NODE *trueValueNode, AST_NODE *falseValueNode){
    AST_NODE *conditionNode;
    size_t nodeSize = sizeof(AST_NODE);

    if((conditionNode = calloc(nodeSize, 1)) == NULL){
        yyerror("Memory allocation failed!");
        exit(1);
    }

    conditionNode->type = CONDITIONAL_NODE_TYPE;
    conditionNode->data.conditional = *createASTConditional(conditionNode, trueOrFalseCondition, trueValueNode,
                                                            falseValueNode);

    return conditionNode;
}

AST_CONDITIONAL *createASTConditional(AST_NODE *conditionNode, AST_NODE *trueOrFalseCondition, AST_NODE *trueValueNode, AST_NODE *falseValueNode){
    AST_CONDITIONAL *astConditional;
    size_t nodeSizeAstCond = sizeof(AST_CONDITIONAL);

    if((astConditional = calloc(nodeSizeAstCond, 1)) == NULL){
        yyerror("Memory allocation failed!");
        exit(1);
    }

    astConditional->condition = trueOrFalseCondition;
    astConditional->trueValue = trueValueNode;
    astConditional->falseValue = falseValueNode;

    trueOrFalseCondition->parent = conditionNode;
    trueValueNode->parent = conditionNode;
    falseValueNode->parent = conditionNode;

    return astConditional;
}

SYMBOL_TABLE_NODE *addLetToList(SYMBOL_TABLE_NODE *newLet, SYMBOL_TABLE_NODE *letList){
    SYMBOL_TABLE_NODE  *tmpList;
    tmpList = letList;

    while(tmpList){
        if(strcmp(newLet->id, tmpList->id) == 0){
            warning("Duplicate assignment to symbol detected in the same scope! Only the first assignment is kept!");
            free(tmpList->value);
            tmpList->value = newLet->value;
            newLet->value = NULL;
            freeSymbolTableNode(newLet);
            return letList;
        }
        tmpList = letList->next;
    }

    newLet->next = letList;

    return newLet;
}

AST_NODE *createNumberNode(double value, NUM_TYPE type){
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // Populate "node", the AST_NODE * created above with the argument data.
    // node is a generic AST_NODE, don't forget to specify it is of numType NUMBER_NODE
    node->type = NUM_NODE_TYPE;
    node->data.number.value = value;
    node->data.number.type = type;

    return node;
}

AST_NODE *createFunctionNode(FUNC_TYPE func, AST_NODE *opList){
    AST_NODE *node;
    size_t nodeSize;

    nodeSize = sizeof(AST_NODE);
    if ((node = calloc(nodeSize, 1)) == NULL)
    {
        yyerror("Memory allocation failed!");
        exit(1);
    }

    // Populate the allocated AST_NODE *node's data
    node->type = FUNC_NODE_TYPE;
    node->data.function.func = func;
    node->data.function.opList = opList;

    while (opList){
        opList->parent = node;
        opList = opList->next;
    }

    return node;
}

AST_NODE *addExpressionToList(AST_NODE *newExpr, AST_NODE *exprList){
    // at newExpr to the exprList as the head. return the resulting list's head.
    newExpr->next = exprList;

    return newExpr; // stub
}

RET_VAL evalNegFunc(AST_NODE *opList){
    if(!opList){
        warning("neg called with no operands! nan returned!");
        return NAN_RET_VAL;
    }else if(opList->next){
        warning("neg called with extra (ignored) operands!");
    }


    AST_NUMBER result, current;
    current = eval(opList);
    result.type = current.type;
    result.value = -current.value;

    return result;
}

RET_VAL evalAbsFunc(AST_NODE *opList){
    if(!opList){
        warning("abs called with no operands! nan returned!");
        return NAN_RET_VAL;
    }else if(opList->next){
        warning("abs called with extra (ignored) operands!");
    }

    AST_NUMBER result, current;
    current = eval(opList);
    result.type = current.type;
    result.value = fabs(current.value);

    return result;
}

RET_VAL evalAddFunc(AST_NODE *opList){
    if(!opList){
        warning("add called with no operands, 0 returned!");
        return ZERO_RET_VAL;
    }

    AST_NUMBER sum, current; // current holds numType: int, double
    sum = ZERO_RET_VAL;

    while(opList){
        current = eval(opList);
        sum.type = sum.type || current.type; // keeps our numType
        sum.value = sum.value + current.value; // probably losses numType
        opList = opList->next;
    }

    return sum;
}

/*
 * subtracts two numbers
 * if not two numbers, gives warning
 */
RET_VAL evalSubFunc(AST_NODE *opList){
    if(!opList){
        warning("sub called with no operands, 0 returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER sum, current; // current holds numType: int, double
    current = eval(opList);
    sum.type = current.type; ////
    sum.value = current.value; // stores first #'s value

    if(opList->next) {
        opList = opList->next;
        current = eval(opList);
        sum.type = sum.type || current.type;
        sum.value = sum.value - current.value;
    } else{
        warning("sub called with only one arg!");
        return NAN_RET_VAL;
    }

    if (opList->next){
        warning("sub called with extra (ignored) operands!");
    }

    return sum;
}

RET_VAL evalMultFunc(AST_NODE *opList){
    if(!opList){
        warning("mult called with no operands, 1 returned!");
        return ONE_RET_VAL;
    }

    AST_NUMBER product, current;
    current = eval(opList); ////
    product.type = current.type;
    product.value = current.value;

    if(opList->next){
        opList = opList->next;
        while(opList){
            current = eval(opList);
            product.type = product.type || current.type;
            product.value = product.value * current.value;
            opList = opList->next;
        }
    }

    return product;
}

/*
 * strict division, int/int cannot equal float
 * // TODO
 * NEEDS ATTENTION
 */
RET_VAL evalDivFunc(AST_NODE *opList){
    if(!opList){
        warning("div called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER quotient, current;
    current = eval(opList);
    quotient = current;

    if(opList->next){
        opList = opList->next;
        current = eval(opList);
        quotient.type = quotient.type || current.type;
        quotient.value = quotient.value / current.value;
    }else{
        warning("div called with only one arg! nan returned!");
        return NAN_RET_VAL;
    }

    if (opList->next){
        warning("div called with extra (ignored) operands!");
    }

    if(quotient.type == INT_TYPE){
        quotient.value = trunc(quotient.value); // handles odd case of rounding
    }

    return quotient;
}

/*
 * binary (a and b input)
 * returns an r such that:
 * a = q * b + r
 * q is an integer
 * 0 <= r < | b |
 *
 * watch out for negative first #
 */
RET_VAL evalRemainderFunc(AST_NODE *opList){
    if(!opList){
        warning("remainder called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER remainder, current;
    current = eval(opList);
    remainder.type = current.type;
    remainder.value = current.value;

    if(opList->next){
        opList = opList->next;
        current = eval(opList);
        remainder.type = remainder.type || current.type;
        remainder.value = fmod(remainder.value,current.value);
    }else{
        warning("remainder called with one arg! nan returned!");
        return NAN_RET_VAL;
    }

    if (opList->next){
        warning("remainder called with extra (ignored) operands!");
    }

    if (remainder.value < 0){
        remainder.value += fabs(current.value);
    }

    return remainder;
}

/*
 * returns e raised to the specified power
 * always returns a double
 */
RET_VAL evalExpFunc(AST_NODE *opList){
    if(!opList){
        warning("exp called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER result, current;
    current = eval(opList);
    result.type = DOUBLE_TYPE;
    result.value = exp(current.value);

    if (opList->next){
        warning("exp called with extra (ignored) operands!");
    }

    return result;
}

/*
 * returns 2 raised to the specified power
 * if operand is negative, return numType is always double
 * otherwise, return numType depends on operand numType
 */
RET_VAL evalExp2Func(AST_NODE *opList){
    if(!opList){
        warning("exp2 called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER result, current;
    current = eval(opList);

    if(current.value < 0){
        result.type = DOUBLE_TYPE;
    } else{
        result.type = current.type;
    }

    result.value = pow(2,current.value);


    if (opList->next){
        warning("exp2 called with extra (ignored) operands!");
    }

    return result;
}

/*
 * returns first operand raised to power of second operand
 */
RET_VAL evalPowFunc(AST_NODE *opList){
    if(!opList){
        warning("pow called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER result, base, power;
    base = eval(opList);

    if(opList->next){
        opList = opList->next;
        power = eval(opList);
        result.type = base.type || power.type;
        result.value = pow(base.value, power.value);

    } else{
        warning("pow called with only one operand! nan returned!");
        return NAN_RET_VAL;
    }


    if (opList->next){
        warning("pow called with extra (ignored) operands!");
    }

    return result;
}

RET_VAL evalLogFunc(AST_NODE *opList){
    if(!opList){
        warning("log called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER result, current;
    current = eval(opList);
    result.type = DOUBLE_TYPE;

    if(current.value < 0){
        return NAN_RET_VAL;
    } else{
        result.value = log(current.value);
    }

    if (opList->next){
        warning("log called with extra (ignored) operands!");
    }

    return result;
}

RET_VAL evalSqrtFunc(AST_NODE *opList){
    if(!opList){
        warning("sqrt called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER result, current;
    current = eval(opList);
    result.type = DOUBLE_TYPE;

    if(current.value < 0){
        return NAN_RET_VAL;
    } else{
        result.value = sqrt(current.value);
    }

    if (opList->next){
        warning("sqrt called with extra (ignored) operands!");
    }

    return result;
}

RET_VAL evalCbrtFunc(AST_NODE *opList){
    if(!opList){
        warning("cbrt called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER result, current;
    current = eval(opList);
    result.type = DOUBLE_TYPE;

    result.value = cbrt(current.value);


    if (opList->next){
        warning("cbrt called with extra (ignored) operands!");
    }

    return result;
}

/*
 * returns the square root of the sum of squares of operands
 *
 */
RET_VAL evalHypotFunc(AST_NODE *opList){
    if(!opList){
        warning("hypot called with no operands, 0 returned!");
        return ZERO_DOUBLE_RET_VAL;
    }

    AST_NUMBER result, current;
    current = eval(opList);
    result.type = DOUBLE_TYPE;
    result.value = pow(current.value,2);

    if(opList->next){
        opList = opList->next;
        while(opList){
            current = eval(opList);
            result.value = result.value + pow(current.value,2);
            opList = opList->next;
        }
    }

    result.value = sqrt(result.value);

    return result;
}

RET_VAL evalMaxFunc(AST_NODE *opList){
    if(!opList){
        warning("max called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER maximum, current;
    current = eval(opList);
    maximum.type = current.type;
    maximum.value = current.value;

    if(opList->next){
        opList = opList->next;
        while(opList){
            current = eval(opList);
            if(current.value > maximum.value){
                maximum.type = current.type;
                maximum.value = current.value;
            }
            opList = opList->next;
        }
    }

    return maximum;
}

RET_VAL evalMinFunc(AST_NODE *opList){
    if(!opList){
        warning("min called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    AST_NUMBER minumum, current;
    current = eval(opList);
    minumum.type = current.type;
    minumum.value = current.value;

    if(opList->next){
        opList = opList->next;
        while(opList){
            current = eval(opList);
            if(current.value < minumum.value){
                minumum.type = current.type;
                minumum.value = current.value;
            }
            opList = opList->next;
        }
    }

    return minumum;
}

RET_VAL evalRandFunc(AST_NODE *opList){
    RET_VAL randomNumber;

    randomNumber.value = (double)rand()/ (double)RAND_MAX;
    return randomNumber;
}

RET_VAL evalReadFunc(AST_NODE *opList){
    RET_VAL retVal;
    char buffer[256];

    fgets(buffer, 256, read_target); // file that already opened in main
    printf("read :: %s", buffer);

    retVal.type = DOUBLE_TYPE;
    int alphabet = 0;
    int proofDouble = 0; // check if int or double

    // check if buffer holds string
    for (int i = 0; buffer[i] != '\0'; i++){
        if(isalpha(buffer[i]) != 0){
            alphabet++;
        } else if(buffer[i] == '.'){
            proofDouble++;
        }
    }

    if(alphabet){
        warning("Invalid read entry! NAN returned!");
        return NAN_RET_VAL;
    } else if(buffer[0] == '.'){
        warning("Invalid read entry! NAN returned!");
        return NAN_RET_VAL; // grammar won't allow leading .
    }


    if(proofDouble){
        retVal.value = atof(buffer);
    } else{
        retVal.type = INT_TYPE;
        retVal.value = atoi(buffer);
    }

    return retVal;

}

RET_VAL evalEqualFunc(AST_NODE *opList){
    if(!opList){
        warning("equal called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    RET_VAL retVal, first, second;
    first = eval(opList);
    second = eval(opList->next);
    retVal.type = INT_TYPE;

    if(first.value == second.value) {
        retVal.value = 1;
    } else {
        retVal.value = 0;
    }

    return retVal;
}

RET_VAL evalLessFunc(AST_NODE *opList){
    if(!opList){
        warning("less called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    RET_VAL retVal, first, second;
    first = eval(opList);
    second = eval(opList->next);
    retVal.type = INT_TYPE;

    if(first.value < second.value) {
        retVal.value = 1;
    } else {
        retVal.value = 0;
    }

    return retVal;
}

RET_VAL evalGreaterFunc(AST_NODE *opList){
    if(!opList){
        warning("greater called with no operands, nan returned!");
        return NAN_RET_VAL;
    }

    RET_VAL retVal, first, second;
    first = eval(opList);
    second = eval(opList->next);
    retVal.type = INT_TYPE;

    if(first.value > second.value) {
        retVal.value = 1;
    } else {
        retVal.value = 0;
    }

    return retVal;
}

RET_VAL evalPrintFunc(AST_NODE *opList){
    if(!opList){
        warning("print called with no operands, nan returned!");
        return NAN_RET_VAL;
    } else if(opList->next) {
        warning("print called with extra (ignored) operands!");
    }

    printRetVal(opList->data.number);

    return opList->data.number;
}

RET_VAL  evalCustomFunc(AST_NODE *node){
    return NAN_RET_VAL;
}

RET_VAL evalFuncNode(AST_NODE *node){
    if (!node)
    {
        yyerror("NULL ast node passed into evalFuncNode!");
        return NAN_RET_VAL; // unreachable but kills a clang-tidy warning
    }

    /* HINT: the helper functions that it calls will need to be defined above it
     because they are not declared in the .h file (and should not be)
     evalFunc will be calc :)
    */
    switch (node->data.function.func) {
        case NEG_FUNC:
            return evalNegFunc(node->data.function.opList);
        case ABS_FUNC:
            return evalAbsFunc(node->data.function.opList);
        case ADD_FUNC:
            return evalAddFunc(node->data.function.opList);
        case SUB_FUNC:
            return evalSubFunc(node->data.function.opList);
        case MULT_FUNC:
            return evalMultFunc(node->data.function.opList);
        case DIV_FUNC:
            return evalDivFunc(node->data.function.opList);
        case REMAINDER_FUNC:
            return evalRemainderFunc(node->data.function.opList);
        case EXP_FUNC:
            return evalExpFunc(node->data.function.opList);
        case EXP2_FUNC:
            return evalExp2Func(node->data.function.opList);
        case POW_FUNC:
            return evalPowFunc(node->data.function.opList);
        case LOG_FUNC:
            return evalLogFunc(node->data.function.opList);
        case SQRT_FUNC:
            return evalSqrtFunc(node->data.function.opList);
        case CBRT_FUNC:
            return evalCbrtFunc(node->data.function.opList);
        case HYPOT_FUNC:
            return evalHypotFunc(node->data.function.opList);
        case MAX_FUNC:
            return evalMaxFunc(node->data.function.opList);
        case MIN_FUNC:
            return evalMinFunc(node->data.function.opList);
        case RAND_FUNC:
            return evalRandFunc(node->data.function.opList);
        case READ_FUNC:
            return evalReadFunc(node->data.function.opList);
        case EQUAL_FUNC:
            return evalEqualFunc(node->data.function.opList);
        case LESS_FUNC:
            return evalLessFunc(node->data.function.opList);
        case GREATER_FUNC:
            return evalGreaterFunc(node->data.function.opList);
        case PRINT_FUNC:
            return evalPrintFunc(node->data.function.opList);
        case CUSTOM_FUNC:
            return evalCustomFunc(node->data.function.opList);
    }

    return NAN_RET_VAL;
}

RET_VAL evalNumNode(AST_NODE *node){
    if (!node)
    {
        yyerror("NULL ast node passed into evalNumNode!");
        return NAN_RET_VAL;
    }

    RET_VAL retVal;
    retVal.type = node->data.number.type;
    retVal.value = node->data.number.value;

    return retVal;
}

RET_VAL intergerADD(AST_NODE *opList){
    if(!opList){
        warning("add called with no operands, 0 returned!");
        return ZERO_RET_VAL;
    }

    AST_NUMBER sum, current; // current holds numType: int, double
    sum = ZERO_RET_VAL;

    while(opList){
        current = eval(opList);
        sum.type = sum.type || current.type; // keeps our numType
        sum.value = sum.value + current.value; // probably losses numType
        opList = opList->next;
    }

    return sum;
}
RET_VAL typeCastCheck(RET_VAL retVal, SYMBOL_TABLE_NODE *curSymbolTable){
    RET_VAL retValCasted = retVal;

    if(curSymbolTable->numType != NO_TYPE) {
        retValCasted.type = curSymbolTable->numType;
    }

    if(retValCasted.type == INT_TYPE){
        retValCasted.value = trunc(retVal.value);
    }

    if(retVal.type == DOUBLE_TYPE && curSymbolTable->numType == INT_TYPE){
        printf("Warning: Precision loss on int cast from %f to %.0f.\n", retVal.value, retValCasted.value);
    }

    return retValCasted;
}

void *checkSymbolTableValueTypeHasNumNodeType(SYMBOL_TABLE_NODE *curSymbolTable, RET_VAL retVal){

    if(curSymbolTable->value->type != NUM_NODE_TYPE){
        freeNode(curSymbolTable->value);
        curSymbolTable->value = createNumberNode(retVal.value, retVal.type);
    }
};

RET_VAL evalSymbolNode(AST_NODE *node){
    if (!node) {
        yyerror("NULL ast node passed into evalSymbolNode!");
        return NAN_RET_VAL; // unreachable but kills a clang-tidy warning
    }

    AST_NODE *curNode = node;
    SYMBOL_TABLE_NODE *curSymbolTable;
    RET_VAL retVal;

    while(curNode){
        curSymbolTable = curNode->variableTable;
        while(curSymbolTable){
            if(strcmp(curSymbolTable->id, node->data.symbol.id) == 0){
                retVal = eval(curSymbolTable->value);
                retVal = typeCastCheck(retVal, curSymbolTable);
                checkSymbolTableValueTypeHasNumNodeType(curSymbolTable, retVal);
                return retVal;
            }
            curSymbolTable = curSymbolTable->next;
        }
        curNode = curNode->parent;
    }

    warning("Undefined symbol evaluated! NAN returned!"); // needs symbol added

    return NAN_RET_VAL;
}

RET_VAL evalScopeNode(AST_NODE *node){
    if (!node) {
        yyerror("NULL ast node passed into evalScopeNode!");
        return NAN_RET_VAL; // unreachable but kills a clang-tidy warning
    }

    RET_VAL returnNode;
    returnNode = eval(node->data.scope.child);

    return returnNode;
}

RET_VAL evalConditionNode(AST_NODE *node){
    if (!node) {
        yyerror("NULL ast node passed into evalConditionNode!");
        return NAN_RET_VAL; // unreachable but kills a clang-tidy warning
    }

    RET_VAL retVal, conditionValue;
    AST_NODE *cond;
    cond = node;
    conditionValue = eval(node->data.conditional.condition);

    if(conditionValue.value == 0){
        retVal = eval(cond->data.conditional.falseValue);
    } else{
        retVal = eval(cond->data.conditional.trueValue);
    }

    return retVal;
}

RET_VAL eval(AST_NODE *node){
    if (!node){
        yyerror("NULL ast node passed into eval!");
        return NAN_RET_VAL;
    }

    switch (node->type) {
        case FUNC_NODE_TYPE:
            return evalFuncNode(node);
        case NUM_NODE_TYPE:
            return evalNumNode(node);
        case SYM_NODE_TYPE:
            return evalSymbolNode(node);
        case SCOPE_NODE_TYPE:
            return evalScopeNode(node);
        case CONDITIONAL_NODE_TYPE:
            return evalConditionNode(node);
    }

    return NAN_RET_VAL;
}

void printRetVal(RET_VAL val){
    switch (val.type) {
        case INT_TYPE:
            printf("Integer : %.lf\n", val.value);
            break;
        case DOUBLE_TYPE:
            printf("Double : %lf\n", val.value);
            break;
        default:
            printf("No Type : %lf\n", val.value);
            break;
    }
}

void freeNode(AST_NODE *node){
    if (!node) {
        return;
    }


    /*
     * look through the AST_NODE struct, decide what referenced data should have freeNode called on it
     * (hint: it might be part of an s_expr_list, with more nodes following it in the list)
     *
     * if this node is FUNC_TYPE, it might have some operands
     * to free as well (but this should probably be done in
     * a call to another function, named something like freeFunctionNode)
     */
    switch (node->type) {
        case FUNC_NODE_TYPE:
            freeFunctionNode(node);
            break;
        case NUM_NODE_TYPE:
            break;
        case SYM_NODE_TYPE:
            freeSymbolNode(node);
            break;
        case SCOPE_NODE_TYPE:
            freeScopeNode(node);
            break;
        case CONDITIONAL_NODE_TYPE:
            freeConditionNode(node);
            break;
    }

    freeNode(node->next);
    freeSymbolTableNode(node->variableTable);

    free(node);
}

void freeSymbolTableNode(SYMBOL_TABLE_NODE *symbolTableNode){
    if(!symbolTableNode){
        return;
    }

    free(symbolTableNode->id);
    freeNode(symbolTableNode->value);
    free(symbolTableNode);
}

void freeFunctionNode(AST_NODE *node){
    freeNode(node->data.function.opList);
}

void freeSymbolNode(AST_NODE *node){
    free(node->data.symbol.id);
}

void freeScopeNode(AST_NODE *node){
    free(node->data.scope.child);
}

void freeConditionNode(AST_NODE *node){
    freeNode(node->data.conditional.condition);
    freeNode(node->data.conditional.trueValue);
    freeNode(node->data.conditional.falseValue);
}

