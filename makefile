CC = g++
CFLAGS = -Iincludes -std=c++17 -Wall

ifeq ($(OS),Windows_NT)
LIBS = -lglfw3 -lopengl32 -lgdi32
RM = del /Q
TARGET = program.exe
else
LIBS = -lglfw -lGL -ldl -lm -lX11 -lpthread
RM = rm -f
TARGET = program
endif

$(TARGET): main.o glad.o shader.o
	$(CC) main.o glad.o shader.o -o $(TARGET) $(LIBS)

main.o: source/main.cpp
	$(CC) $(CFLAGS) -c source/main.cpp

shader.o: source/shader.cpp
	$(CC) $(CFLAGS) -c source/shader.cpp

glad.o: source/glad.c
	gcc $(CFLAGS) -c source/glad.c

clean:
	$(RM) *.o $(TARGET)