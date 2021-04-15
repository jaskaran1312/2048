//Bookkeeping
int direction;
int operation;
int op;
int loc;
int x;
int y;
int num;
char *id;
int err;

typedef struct nodeStruct{
	char* id;
	struct nodeStruct *next;
} node;

/*
 typedef struct varStruct{
	int idx;
	char* id;
	struct varStruct *next;
} var;
*/
typedef struct cellStruct{
	int val;
	struct nodeStruct *label;
} cell;

typedef struct boardStruct{
//	struct varStruct *vars;		
	struct cellStruct* cells[16];	
} board;

//Utility
board *init();
void printBoard(board *b);
void printState(board *b);
int checkPair(int x, int y);

//Board manipulation
void printLabel(cell *c, int idx);
void addLabel(cell* c, char* id);
void dropLabels(cell* c);
void concatLabels(cell *a, cell *b);
int getIndex(int, int);
int getIndexByLabel(board *b, char *id);
//int getIndex(var* v, char *id);
int getValue(cell *c);
//void addLabel(var* v, char* id, int idx);
int findLabel(cell *c, char *id);
int addRandomTile(board *b);

int compute(int, int);

//Handlers
void move(int, int, board*, int);
void assign(cell *c, int val);
void name(cell *c, char *id);
int query(cell *c);

//Decides handler
int action(int op);

//Print functions
void red();
void redf();
void green();
void greenf();
void cyan();
void reset();
void prompt();
void printHelp();
