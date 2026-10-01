program: main.o glad.o
	g++ main.o glad.o -o program -lglfw -lGL -ldl -lm -lX11 -lpthread

main.o: source/main.cpp
	g++ -Iincludes -DGLFW_INCLUDE_NONE -c source/main.cpp -o main.o

glad.o: source/glad.c
	gcc -Iincludes -c source/glad.c -o glad.o

clean:
	rm -f program main.o glad.o