all: clean build run

win: clean wbuild run

clean:
	rm target/trijam15.exe

build:
	gcc -g src/main.c -o target/trijam15.exe -lSDL2 -lSDL2_ttf 

wbuild:
	gcc -g src/main.c -o target/trijam15.exe -Incurses src/*.h -lmingw32 -lm -lSDL2main -lSDL2 -lSDL2_ttf 

run:
	'./target/trijam15.exe'

windows_release:
	gcc src/main.c -o release/trijam15.exe -lmingw32 -lm -lSDL2main -lSDL2 -lSDL2_ttf

linux_release:
	gcc src/main.c -o release/trijam15 -lSDL2 -lSDL2_ttf