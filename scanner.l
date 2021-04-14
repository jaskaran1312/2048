%{
#include "parser.tab.h"	
%}
whitespace [ \t\r]+
alphabet [a-zA-Z]
%%
ADD	    	{ 
	#ifdef DEBUG
		printf("ADD\n"); 
	#endif 
	return ADD; }
SUBTRACT 	{
	#ifdef DEBUG
		 printf("SUB\n");
	#endif
	return SUB; }

MULTIPLY    	{ 
	#ifdef DEBUG
 		printf("MUL\n");
	#endif
	return MUL; }

DIVIDE		{ 
	#ifdef DEBUG
 		printf("DIV\n");
	#endif
	return DIV; }

LEFT    	{
 	#ifdef DEBUG
		 printf("LEFT\n");
	#endif
	return LEFT; }

RIGHT    	{ 
	#ifdef DEBUG
 		printf("RIGHT\n");
	#endif
	return RIGHT; }

UP	    	{ 
	#ifdef DEBUG
 		printf("UP\n");
	#endif
	return UP; }

DOWN    	{ 
	#ifdef DEBUG
 		printf("DOWN\n");
	#endif
	return DOWN; }

[0-9]+		{ 
	#ifdef DEBUG
 		printf("num\n");
	#endif
	yylval.num = atoi(yytext); 
	return NUMBER;}

TO		{ 
	#ifdef DEBUG
 		printf("TO\n");
	#endif
	return TO; }

ASSIGN		{ 
	#ifdef DEBUG
 		printf("ASSIGN\n");
	#endif
	return ASSIGN; }

VAR		{ 
	#ifdef DEBUG
 		printf("VAR\n");
	#endif
	return VAR; }

IS		{ 
	#ifdef DEBUG
 		printf("IS\n");
	#endif
	return IS; }

VALUE		{ 
	#ifdef DEBUG
 		printf("VALUE\n");
	#endif
	return VALUE; }

IN		{ 
	#ifdef DEBUG
 		printf("IN\n");
	#endif
	return IN; }

HELP		{ 
	#ifdef DEBUG
 		printf("HELP\n");
	#endif
	return HELP; }

,		{ 
	#ifdef DEBUG
 		printf("comma\n");
	#endif
	return COMMA; }

\.		{ 
	#ifdef DEBUG
 		printf("dot\n");
	#endif
	return PERIOD; }

{alphabet}+	{ 
	#ifdef DEBUG
 		printf("id\n"); 
	#endif
	yylval.id = yytext;
	return ID; }

\n     		{ 
	#ifdef DEBUG
 		printf("linefeed\n");
	#endif
	return EOL; }

{whitespace}	{ 
	#ifdef DEBUG
 		printf("space\n");
	#endif
	}

.      		{ 
	#ifdef DEBUG
 		printf("unitentified\n");
	#endif
	return UNDEF;
	}
%%
