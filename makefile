CC = g++
CFLAGS = -Iincludes -std=c++17 -Wall
LIBS = -lglfw -lGL -ldl -lm -lX11 -lpthread

program: main.o glad.o shader.o
	$(CC) main.o glad.o shader.o -o program $(LIBS)

main.o: source/main.cpp
	$(CC) $(CFLAGS) -c source/main.cpp

shader.o: source/shader.cpp
	$(CC) $(CFLAGS) -c source/shader.cpp

glad.o: source/glad.c
	gcc $(CFLAGS) -c source/glad.c

clean:
	rm -f *.o program