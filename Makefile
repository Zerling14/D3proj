all: window

debug: build/debug.o build/D3Tool.o
	gcc -Wall -Werror -o bin/debug build/debug.o build/D3Tool.o -lm
window: build/window.o build/D3Tool.o
	g++ -Wall -o bin/window build/window.o build/D3Tool.o -lm -lgdi32 -static-libgcc -static-libstdc++
build/window.o: src/window.c
	g++ -Wall -c -o build/window.o src/window.c
build/D3Tool.o: src/D3Tool.c
	gcc -Wall -c -o build/D3Tool.o src/D3Tool.c
build/debug.o: src/debug.c
	gcc -Wall -c -o build/debug.o src/debug.c
	
