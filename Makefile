customeval: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DCUSTOM -DEVAL
	clear
	./exe
customdebug: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DCUSTOM -DDEBUG -DEVAL
	clear
	./exe

custom: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DCUSTOM
	clear
	./exe

eval: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DEVAL
	clear
	./exe

debug: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DDEBUG
	clear
	./exe

all: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly
	clear
	./exe

scanner:
	flex scanner.l

parser:
	bison -d parser.y

clean:
	rm -rf lex.yy.c parser.tab.h parser.tab.c exe
