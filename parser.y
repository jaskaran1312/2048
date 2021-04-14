%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_modules.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

board* b;

%}

%union {
	int num;
	char *id;
}

%token ADD SUB MUL DIV
%token LEFT RIGHT UP DOWN
%token<num> NUMBER
%token<id> ID
%token ASSIGN TO VAR IS VALUE IN
%token COMMA PERIOD
%token HELP
%token UNDEF
%token EOL

%type<num> query assignable
%start command

%%

command:
       | expression PERIOD EOL 	{ action(op); printBoard(b); return 0; }
       | expression EOL 	{ red(); printf("Syntax Error. Hint: use a Period \".\" at the end.\n"); fflush(stdout); reset(); prompt(); return 0; }
       | HELP PERIOD EOL	{ action(7); return 0; }			
       | HELP EOL	 	{ red(); printf("Syntax Error. Hint: use a Period \".\" at the end.\n"); fflush(stdout); reset(); prompt(); return 0; }
       | err EOL	 	{ red(); printf("Syntax Error. Hint: type 'HELP.' for usage.\n"); fflush(stdout); reset(); prompt(); return 0; }
       | EOL			{ prompt(); return 0; }
       | keywords EOL	 	{ red(); printf("Syntax Error. Hint: type 'HELP.' for usage.\n"); fflush(stdout); reset(); prompt(); return 0; }

;

err: ID
   | UNDEF
   | PERIOD
   | COMMA
;

keywords: ADD
	| SUB
	| MUL
	| DIV
	| LEFT
	| RIGHT
	| UP
	| DOWN
	| TO
	| ASSIGN
	| VAR
	| IS
	| VALUE
	| IN
;
expression: move		{ op = 1; }
  	  | assignment		{ op = 2; } 
       	  | naming		{ op = 3; }
       	  | query		{ op = 4; }
;

move: operation direction
    | operation ID		{red(); printf("Syntax Error. Hint: Use uppercase for direction.\n"); reset(); prompt(); return 0;}
    | ID direction		{red(); printf("Syntax Error. Hint: Use uppercase for operation.\n"); reset(); prompt(); return 0;}	    
;

assignment: ASSIGN assignable TO location 	{ num=$2; }
	  | ID assignable TO location		{ red(); printf("Syntax Error. Hint: Use uppercase for keyword 'ASSIGN'.\n"); reset(); prompt(); return 0;}	    
	  | ASSIGN assignable ID location	{ red(); printf("Syntax Error. Hint: Use uppercase for keyword 'TO'.\n"); reset(); prompt(); return 0;}	    
	  | ID assignable ID location 		{ red(); printf("Syntax Error. Hint: Use uppercase for keywords 'ASSIGN' and 'TO'.\n"); reset(); prompt(); return 0;}	    
;

assignable: NUMBER	{$$=$1;}
;

query: VALUE IN location { $$=action(5); }
;

location: pair		{ loc=1; }
    	| ID		{ id=$1; loc=0; }
;

naming: VAR ID IS pair { id=$2;}
;

pair: NUMBER COMMA NUMBER { x = $1; y = $3; if(action(6)) return 0; }
    | ID COMMA NUMBER { red(); printf("Syntax Error. Please type <num>,<num>\n"); reset(); prompt(); return 0;}

operation: ADD		{ operation=0; }
	 | SUB		{ operation=1; }
         | MUL		{ operation=2; }
         | DIV		{ operation=3; }
;

direction: LEFT		{ direction=0; }
	 | RIGHT	{ direction=2; }
         | UP		{ direction=1; }
         | DOWN		{ direction=3; }
;


%%

int main() {
	yyin = stdin;
	b = init();
	cyan();
	printf("Welcome to the 2048 interpreter.\nType 'HELP.' for info.\n");
	reset();
	printf("Written by Jaskaran Singh Bhatia in C for CSF363\n\n");
	prompt();
	do {
		yyparse();
#ifdef DEBUG
		printf("Completed!\n");
#endif
} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
#ifdef DEBUG
	printf( "Parse error: %s\n", s);
#endif
}


int action(int op){

	int idx = getIndex(x, y);

	if(op==1){ //MOVE
		move(direction, operation, b);
	}

	else if(op==2){ //ASSIGN
		//loc = 1: pair; loc = 0: var;
		if(loc){
			assign(b->cells[idx], num);
		}
		else{
			char *idptr = strtok(id, " ");
			char *idptr2 = strtok(id, ".");
			char *identifier = malloc(strlen(idptr2));
			strcpy(identifier, idptr2);
				
			idx = getIndexByLabel(b, identifier);
			if(idx==-1){
				red();
				printf("Error. The variable '%s' does not exist!\n", identifier);
				reset();
				return 0;
			}
			else{
				assign(b->cells[idx], num);
				green();
				printf("Assigned %d to %s.\n", num, identifier);
				reset();
			}
			
		}
	}

	else if(op==3){
		char *idptr = strtok(id, " ");
		char *identifier = malloc(strlen(idptr));
		strcpy(identifier, idptr);
		if(getIndexByLabel(b, identifier)!=-1){
			red();
			printf("ERROR: Variable '%s' already exists!\n", identifier);
			reset();
			return 0;
		}
		if(b->cells[idx]->val==0){
			red();
			printf("ERROR: Cannot give name to empty tile!\n");
			reset();
			return 0;
		}
			
		name(b->cells[idx], identifier);
	}

	else if(op==4){
		if(loc){
			green();
			printf("Value at (%d, %d) is %d\n", x, y, query(b->cells[idx]));
			reset();
		}
		else{
			idx = getIndexByLabel(b, id);
			if(idx==-1){
				red();
				printf("Error. The variable does not exist.\n");
				reset();
				return 0;
			}
			else{
				green();
				printf("Value at (%d, %d) is %d\n", x, y, query(b->cells[idx]));
				reset();
			}
	
		}
	}
	else if(op==5){
		return getValue(b->cells[idx]);
	}
	else if(op==6){
		return checkPair(x, y);	
	}

	else if(op==7){
		printHelp();
		prompt();
	}
	
	return 0;
}
