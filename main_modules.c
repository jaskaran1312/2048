#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main_modules.h"

board *init()
{
	board *b = malloc(sizeof(board));

	//	b->vars = malloc(sizeof(var));
	//	b->vars->idx=-1;
	//	b->vars->next=NULL;
	for (int i = 0; i < 16; i++)
	{
		b->cells[i] = malloc(sizeof(cell));
		b->cells[i]->label = malloc(sizeof(node));
		b->cells[i]->val = 0;
		b->cells[i]->label->next = NULL;
	}
#ifdef DEBUG
//	printf("Initialized! Address is %d\n", b);
#endif
	return b;
}

void printBoard(board *b)
{

	printf("\t\u250c");
	for(int i = 0; i<31; i++){
		if(i==7|| i==15 || i== 23) printf("\u252c");
		else printf("\u2500");
	}
	printf("\u2510\n");
	for (int i = 0; i < 4; i++)
	{
		printf("\t\u2502");
		if(b->cells[i * 4 + 0]->val){
			printf(" %5d ", b->cells[i * 4 + 0]->val);
		}
		else{
				printf("       ");
		}
		printf("\u2502");
		
		if(b->cells[i * 4 + 1]->val){
			printf(" %5d ", b->cells[i * 4 + 1]->val);
		}
		else{
				printf("       ");
		}
		printf("\u2502");
		
		if(b->cells[i * 4 + 2]->val){
			printf(" %5d ", b->cells[i * 4 + 2]->val);
		}
		else{
				printf("       ");
		}
		printf("\u2502");
		
		if(b->cells[i * 4 + 3]->val){
			printf(" %5d ", b->cells[i * 4 + 3]->val);
		}
		else{
				printf("       ");
		}
		printf("\u2502\n");
		
		if(i==3) printf("\t\u2514");
		else printf("\t\u251c");
		
		for(int j = 0; j<31; j++){
		
		        if(j==7 || j==15 || j== 23){
				if(i==3)printf("\u2534");
				else printf("\u253c");

			} 
			else printf("\u2500");
		}
		
		if(i==3) printf("\u2518\n\n");
		else printf("\u2524\n");
			
	}


	prompt();
#ifdef DEBUG
	printf("\n");
	for(int i=0; i<16; i++){
		printf("%d: val %d\n", i, b->cells[i]->val);
		//printLabel(b->cells[i], i);
	}
		printf("-------\n");
#endif

}

void printState(board *b){
	for(int i = 0; i<16; i++){
		fprintf(stderr, "%d\40", b->cells[i]->val);
	}
	for(int i = 0; i<16; i++){
		printLabel(b->cells[i], i);
	}
}

int checkPair(int x, int y)
{
	if (x > 0 && x < 5 && y > 0 && y < 5)
	{
		return 0;
	}
	red();
	printf("Error. Expected coordinates in range 1 to 4.\n");
	reset();
	return 1;
}

int getIndex(int x, int y)
{
	return (x - 1) * 4 + (y - 1);
}

int getIndexByLabel(board *b, char *identifier)
{

	for (int i = 0; i < 16; i++)
	{
		if (findLabel(b->cells[i], identifier))
			return i;
	}

	return -1;
}

void addLabel(cell *c, char *id)
{
	node *l = c->label;
	while (l->next)
	{
		l = l->next;
	}
	l->id = id;
	l->next = malloc(sizeof(node));
	l->next->next = NULL;

#ifdef DEBUG
//	printLabel(c);
#endif
}
/*
void addLabel(var* v, char* id, int idx){
	char *idptr = strtok(id, " ");
	char *idptr2 = strtok(id, ".");
	char *identifier = malloc(strlen(idptr2));
	strcpy(identifier, idptr2);
	while(v->next){
		v = v->next;
	}
	v->id = identifier;
	v->idx = idx;
	v->next = malloc(sizeof(var));
	v->next->next=NULL;
}
*/

void dropLabels(cell *c)
{
	node *first = c->label;
	node *l = first->next;
	node *next;
	if(!l) return;
	while (l->next)
	{
		next = l->next;
		free(l->id);
		free(l);
		l = next;
	}
	first->id = NULL;
	first->next = NULL;
	c->label = first;
}

int findLabel(cell *c, char *id)
{
	node *l = c->label;
	while (l->next)
	{
		if (strcmp(l->id, id) == 0)
		{
			return 1;
		}
		l = l->next;
	}
	return 0;
}

void printLabel(cell *c, int idx)
{
	node *l = c->label;
//	printf("%d\n", l);
	while (l->next)
	{
		fprintf(stderr, "%d,%d%s\40",(idx/4)+1, idx%4 +1, l->id);
		l = l->next;
	}
}

int getValue(cell *c)
{
	return c->val;
}

void putValue(cell *c, int val)
{
	c->val = val;
}

void move(int dir, int op, board *b)
{

	char *directions[4] = {"LEFT", "UP", "RIGHT", "DOWN"};
	
	char *operations[4] = {"ADD", "SUB", "MUL", "DIV"};
	
	int directionConstants[4][4] = {{0, 4, 1, 4},{0, 16, 4, 1},{3, -1, -1, 4},{12, -4, -4, 1}};

	int S = directionConstants[dir][0];
	int E = directionConstants[dir][1];
	int I = directionConstants[dir][2];
	int O = directionConstants[dir][3];

	int oldState[16];

	for(int i=0; i<16; i++){
		oldState[i]=b->cells[i]->val;	
	}

	for(int i=0; i<4; i++){
		cell *tempArr[4] = {NULL, NULL, NULL, NULL};
		int curr=0;

		for(int j = S + i*O; j != E + i*O; j+=I){
			if(b->cells[j]->val){
				tempArr[curr] = b->cells[j];
				curr++;
			}
		}
#ifdef DEBUG
		printf("Pre Compression\n");
#endif
		for(int index = 0; index < curr-1 ; index++){
			if(tempArr[index]->val == tempArr[index+1]->val){
				tempArr[index]->val = compute(tempArr[index]->val, op);
				tempArr[index+1]->val = 0;
				concatLabels(tempArr[index], tempArr[index+1]);
				dropLabels(tempArr[index+1]);
			}
		}
#ifdef DEBUG
		printf("Computation\n");
#endif

		
		cell *compTempArr[4] = {NULL, NULL, NULL, NULL};
		int k=0;
		for(int j = 0; j<curr; j++){
			if(tempArr[j]->val){
				compTempArr[k] = tempArr[j];
				k++;
			}

		}
#ifdef DEBUG
		printf("Post Compression\n");
#endif



		k = 0;
		for(int j = S + i*O; j != E + i*O; j+=I){
			if(compTempArr[k]){
				b->cells[j] = compTempArr[k];
				k++;
			}
			else{
				b->cells[j] = malloc(sizeof(cell));
				b->cells[j]->label = malloc(sizeof(node));
				b->cells[j]->val = 0;
				b->cells[j]->label->next = NULL;
			
			}

		}
	
	
	}

	int changed = 0;
	for(int i=0; i<16; i++){
		if(oldState[i]!=b->cells[i]->val){
			changed = 1;
		}
	}

	if(!changed){
		green();
		printf("Performed %s %s. Board hasn't changed.\n", operations[op], directions[dir]);
		reset();
	}

	else{
		if(addRandomTile(b)){
			green();
			printf("Performed %s %s. Board updated. Random tile added.\n", operations[op], directions[dir]);
			reset();
		}
		else {
			green();
			printf("Performed %s %s. Board is full. No random tile added.\n", operations[op], directions[dir]);
			reset();
		}
	}

	fflush(stdout);


}

int addRandomTile(board *b){
	
	int full=1;

	for(int i=0; i<16; i++){
		if(b->cells[i]->val==0){
			full=0;
			break;
		}
	}

	if(full){
		return 0;
	}

	while(1){
		int num = rand()%16;
		if(b->cells[num]->val==0){
			assign(b->cells[num], 2);
			return 1;		
		}

	}

}

void concatLabels(cell *a, cell *b){
	node *l = b->label;
	while(l->next){
		addLabel(a, l->id);
		l = l->next;
	}
}

int compute(int x, int op){
	//ADD SUB MUL DIV
	if(op==0)
		return 2*x;
	else if(op==1)
		return 0;
	else if(op==2)
		return x*x;
	else if(op==3)
		return 1;
	return -1;
}

void assign(cell *c, int val)
{
	putValue(c, val);
}

void name(cell *c, char *identifier)
{
	green();
	printf("Setting (%d, %d) to \"%s\"\n", x, y, identifier);
	reset();
	addLabel(c, identifier);
}

int query(cell *c)
{
	return getValue(c);
}

void red()
{
	printf("\033[1;31m");
	printf("2048> ");
}

void redf()
{
	printf("\033[1;31m");
}
void green()
{
	printf("\033[1;32m");
	printf("2048> ");
}
void greenf()
{
	printf("\033[1;32m");
}
void cyan()
{
	printf("\033[1;33m");
}
void reset()
{
	printf("\033[0m");
}

void prompt()
{
	printf("----> ");
}

void printHelp()
{
	cyan();
	printf("The 2048 Interpreter\n");
	printf("Written by Jaskaran Singh Bhatia in C for CSF363.\n\n");
	reset();

	greenf();
	printf("Syntax: Keywords are uppercase. End Commands with a Period \".\"\n");
	reset();

	printf("\nCommands:\n\n");

	greenf();
	printf("\t1. MOVES: ");
	reset();
	redf();
	printf("<operation> <direction>");
	reset();
	printf(".\n\t   where operation = {ADD, SUBTRACT, MULTIPLY, DIVIDE}\n\t   and direction = {LEFT, RIGHT, UP, DOWN}\n");

	redf();
	printf("\t   Example: \"ADD LEFT.\"\n\n\n");
	reset();

	greenf();
	printf("\t2. ASSIGNMENT: ");
	reset();
	printf("ASSIGN ");
	redf();
	printf("<value>");
	reset();
	printf(" TO ");
	redf();
	printf("<x>");
	reset();
	printf(",");
	redf();
	printf("<y>");
	reset();
	printf(".\n\t   assigns a value (integer) to the cell at (x,y)\n");

	redf();
	printf("\t   Example: \"ASSIGN 7 TO 2,3.\"\n\n\n");
	reset();

	greenf();
	printf("\t3. NAMING: ");
	reset();
	printf("VAR ");
	redf();
	printf("<varname>");
	reset();
	printf(" IS ");
	redf();
	printf("<x>");
	reset();
	printf(",");
	redf();
	printf("<y>");
	reset();
	printf(".\n\t   assigns a name (alphabet string) to the cell at (x,y)\n");

	redf();
	printf("\t   Example: \"VAR block IS 4,4.\"\n\n\n");
	reset();

	greenf();
	printf("\t4. QUERY: ");
	reset();
	printf("VALUE IN ");
	redf();
	printf("<x>");
	reset();
	printf(",");
	redf();
	printf("<y>");
	reset();
	printf(".\n\t   Prints the value of the tile at (x,y)\n");

	redf();
	printf("\t   Example: \"VALUE IN 1,4.\"\n\n\n");
	reset();
}
