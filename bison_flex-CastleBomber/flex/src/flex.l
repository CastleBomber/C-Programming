/* NOTE: % must be in the first column (no indentation) */
/* TODO add more regular expressions and returns for the other TOKEN types */


%option noyywrap
%option nounput
%option noinput

%{
   #include "flex.h"
%}

letter          [a-zA-z_$]
digit           [0-9]
addop           [+-]
multop          [*/%]

int             {digit}+
float           {digit}+\.{digit}*
ident           {letter}({letter}|{digit})*

%%

"if"        {return IF_TOKEN;}
"while"     {return WHILE_TOKEN;}
"repeat"    {return REPEAT_TOKEN;}
"print"     {return PRINT_TOKEN;}

{int}       {return INT_TOKEN;}
{float}     {return FLOAT_TOKEN;}
{ident}     {return IDENT_TOKEN;}
{addop}     {return ADDOP_TOKEN;}
{multop}    {return MULTOP_TOKEN;}
"("         {return LPAREN_TOKEN;}
")"         {return LPAREN_TOKEN;}
";"         {return SEMICOLON_TOKEN;}
"="         {return ASSIGN_TOKEN;}


<<EOF>>     {return EOF_TOKEN;}
[ \n\r\t]   ; //skip whitespace
.           {printf("ERROR: invalid character: >>%s<<\n", yytext);}

%%

