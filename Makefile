all: clean build run

clean:
	rm target/trijam15.exe

build:
	gcc -g src/main.c -o target/trijam15.exe -lSDL2 -lSDL2_ttf 

wbuild:
	gcc -g src/main.c -o target/trijam15.exe -lmingw32 -lm -lSDL2main -lSDL2 -lSDL2_ttf 

run:
	'./target/trijam15.exe'

