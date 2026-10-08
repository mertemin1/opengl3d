#pragma once
#include <GLFW/glfw3.h>

//not used for now 
//gonna implement this if the glfw part gets too complicated to handle in the main.cpp


class Window{
    public:
        Window(int width , int height , const char* title);
        ~Window();

        bool shouldClose();
        void swapBuffer();
        void pollEvents();

        GLFWwindow* getNativeWindow();
};