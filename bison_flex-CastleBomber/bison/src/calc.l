%option noyywrap

%{
    #include "calc.h"
    #define llog(token) {fprintf(flex_bison_log_file, "LEX: %s \"%s\"\n", #token, yytext);fflush(flex_bison_log_file);}
%}

digit       [0-9]
int         [+-]?{digit}+
float       [+-]?{digit}+\.{digit}*
func        "neg"|"abs"|"exp"|"log"|"sqrt"|"add"|"sub"|"mult"|"div"|"rem"
lparen      [(]
rparen      [)]
quit        "quit"
%%

{int} {
    llog(INT);
    yylval.dval = strtod(yytext, NULL);
    return INT;
}
{float} {
    llog(FLOAT);
    yylval.dval = strtod(yytext, NULL);
    return FLOAT;
}
{func} {
    llog(FUNC);
    yylval.fval = resolveFunc(yytext);
    return FUNC;
}
{lparen} {
    llog(lparen);
    return LPAREN;
}
{rparen} {
    llog(rparen);
    return RPAREN;
}
{quit} {
    llog(QUIT);
    return QUIT;
}

[\n] {
    llog(EOL);
    return EOL;
}

[\r\t ] {}; /* skip whitespace */

. {
    llog(ERROR INVALID);
}

%%