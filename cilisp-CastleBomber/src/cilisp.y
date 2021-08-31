%{
    #include "cilisp.h"
    #define ylog(r, p) {fprintf(flex_bison_log_file, "BISON: %s ::= %s \n", #r, #p); fflush(flex_bison_log_file);}
    int yylex();
    void yyerror(char*, ...);
%}

%union {
    double dval;
    int ival;
    char *id;
    struct ast_node *astNode;
    struct symbol_table_node *symbolTableNode;
}

%token <ival> FUNC TYPE
%token <dval> INT DOUBLE
%token QUIT LPAREN RPAREN EOL EOFT LET COND LAMBDA
%token <id> SYMBOL

%type <astNode> s_expr f_expr s_expr_section s_expr_list number
%type <symbolTableNode> let_section let_list let_elem

%%

program:
    s_expr EOL {
        ylog(program, s_expr EOL);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
        YYACCEPT;
    }
    | s_expr EOFT {
        ylog(program, s_expr EOFT);
        if ($1) {
            printRetVal(eval($1));
            freeNode($1);
        }
        exit(EXIT_SUCCESS);
    }
    | EOL {
        ylog(program, EOL);
        YYACCEPT;  // paranoic; main skips blank lines
    }
    | EOFT {
        ylog(program, EOFT);
        exit(EXIT_SUCCESS);
    };


s_expr:
    f_expr {
        ylog(s_expr, f_expr);
        $$ = $1;
    }
    | number {
        ylog(s_expr, number);
        $$ = $1;
    }
    | SYMBOL {
	ylog(s_expr, SYMBOL);
	$$ = createSymbolASTNode($1);
    }
    | LPAREN let_section s_expr RPAREN {
	ylog(s_expr, LPAREN);
	$$ = createScopeNode($2, $3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN {
    	ylog(s_expr, LPAREN);
    	$$ = createConditionNode($3, $4, $5);
    }
    | QUIT {
        ylog(s_expr, QUIT);
        exit(EXIT_SUCCESS);
    }
    | error {
        ylog(s_expr, error);
        yyerror("unexpected token");
        $$ = NULL;
    };

f_expr:
    LPAREN FUNC s_expr_section RPAREN {
        ylog(f_expr, LPRAREN);
        $$ = createFunctionNode($2, $3);
    }
    | LPAREN SYMBOL s_expr_section RPAREN {
         ylog(f_expr, LPAREN);
         $$ = NULL;
    };

s_expr_section:
    s_expr_list {
	ylog(s_exp_section, s_exp_list);
	$$ = $1;
    }
    | /* empty */ {
    	ylog(s_expr_section, NULL);
    	$$ = NULL;
    };

s_expr_list:
    s_expr {
	ylog(s_expr_list, s_expr);
	$$ = $1;
    }
    | s_expr s_expr_list{
	ylog(s_expr_list, s_expr);
	$$ = addExpressionToList($1, $2);
    };

arg_list:
    SYMBOL {
	ylog(arg_list, SYMBOL);
	//$$ = NULL;
    }
    | SYMBOL arg_list{
	ylog(arg_list, SYMBOL);
	//$$ = NULL;
    }
    | /* empty */ {
        ylog(arg_list, NULL);
        //$$ = NULL;
    };

let_section:
    LPAREN LET let_list  RPAREN {
        ylog(let_section, LPAREN);
	$$ = $3;
    };

let_list:
     let_elem {
        ylog(let_list, let_elem);
        $$ = $1;
    }
    | let_elem let_list{
    	ylog(let_list, let_elem);
    	$$ = addLetToList($1, $2);
    };

let_elem:
    LPAREN SYMBOL s_expr  RPAREN {
        ylog(let_elem, LPAREN);
        $$ = createSymbolTableNode($2, $3);
    }
    | LPAREN TYPE SYMBOL s_expr RPAREN{
        ylog(TYPE, LPAREN);
        $$ = createSymbolTableNodeWithType($3, $4, $2);
    }
    | LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN {
        ylog(let_elem, LPAREN);
        $$ = NULL;
    }
    | LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr LPAREN {
        ylog(let_elem, LAPREN);
        $$ = NULL;
    };

number:
    INT {
        ylog(number, INT);
        $$ = createNumberNode($1, INT_TYPE);
    }
    | DOUBLE {
	ylog(number, DOUBLE);
	$$ = createNumberNode($1, DOUBLE_TYPE);
    };

%%

