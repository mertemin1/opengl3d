#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
class Shader{
    public: 
        unsigned int ID;
        //we need to hold a aspectratio var for the perspective matrix.
        static float aspectRatio;
        Shader (const char* vertexPath , const char* fragmentPath);
        void use();
        void setColor();
        
};