#include "window.hpp"

Window::Window(int width , int height ,const char* title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width , height , title , NULL , NULL);
}