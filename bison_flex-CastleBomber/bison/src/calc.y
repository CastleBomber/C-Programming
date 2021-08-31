%{
    #include "calc.h"
    #define ylog(r, p) {fprintf(flex_bison_log_file, "BISON: %s ::= %s \n", #r, #p); fflush(flex_bison_log_file);}
%}

%union
{
    double dval;
    int fval;
    struct number *nval;
}

%token <dval> INT FLOAT
%token <fval> FUNC
%token EOL LPAREN RPAREN QUIT

%type <nval> number expr f_expr

%%

program:
    expr EOL {
        ylog(program, expr EOL);
        printNumber(stdout, $1);
        YYACCEPT;
    }
    | QUIT {
        ylog(program, QUIT);
        exit(0);
    };

expr:
    number {
        ylog(expr, number);
        $$ = $1;
    }
    | f_expr {
        ylog(expr, f_expr);
        $$ = $1;
    };

f_expr:
    LPAREN FUNC expr RPAREN {
	ylog(f_expr, LPAREN);
	$$ = calc($2, $3, NULL);
    }
    | LPAREN FUNC expr expr RPAREN {
	ylog(f_expr, FUNC);
	$$ = calc($2, $3, $4);
    };


number:
    INT {
        ylog(number, INT);
        $$ = createNumber(INT_TYPE, $1);
    }
    | FLOAT {
	ylog(number, FLOAT);
	$$ = createNumber(FLOAT_TYPE, $1);
    };
%%

