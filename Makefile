eval: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DEVAL
	./exe


debug: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly -DDEBUG
	./exe

all: parser scanner
	gcc -o exe lex.yy.c parser.tab.c main_modules.c -ll -ly
	./exe

scanner:
	flex scanner.l

parser:
	bison -d parser.y

clean:
	rm -rf lex.yy.c parser.tab.h parser.tab.c exe
