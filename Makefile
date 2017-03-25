all: window
main: main.c
	gcc -Wall -lm -o main.exe main.c
window: window.c main.c
	g++ -Wall -c -o window.o window.c
	gcc -Wall -c -o main.o main.c
	g++ -Wall -o main window.o main.o -lm -lgdi32 -static-libgcc -static-libstdc++
