build:
	gcc -Iinclude src/* -o bin/main.exe -fopenmp -lpthread -w

run: build 
	bin/main.exe