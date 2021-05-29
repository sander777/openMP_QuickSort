build:
	gcc -Iinclude src/* -o bin/main.exe -fopenmp -lpthread

run: build 
	bin/main.exe