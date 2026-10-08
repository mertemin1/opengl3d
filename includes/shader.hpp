#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
class Shader{
    public: 
        unsigned int ID;
        Shader (const char* vertexPath , const char* fragmentPath);
        void use();
        void setColor();
};