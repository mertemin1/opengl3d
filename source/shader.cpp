#include "shader.hpp"
Shader::Shader(const char* vertexPath , const char* fragmentPath){

    //read the shaders from files
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile; 
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream , fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e ){
        std::cerr << "error in reading shader files" << std::endl;
    
    }
    //opengl works in C style char arrays
    const char* vShaderCode = vertexCode.c_str();
    const char* vFragmentCode = fragmentCode.c_str();

    //compile shaders
    unsigned int vertex, fragment;
    int success; 
    char infoLog[512];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex , 1 , &vShaderCode , NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex , GL_COMPILE_STATUS , &success);
    if(!success){
        glGetShaderInfoLog(vertex , 512 , NULL , infoLog);
        std::cerr << "error in vertex compilation:\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment , 1 , &vFragmentCode , NULL);
    glCompileShader(fragment);
    
    glGetShaderiv(fragment , GL_COMPILE_STATUS , &success);
    if(!success){
        glGetShaderInfoLog(fragment , 512 , NULL , infoLog);
        std::cerr << "error in fragment compilation:\n" << infoLog << std::endl;
        
    }

    //shader program
    ID = glCreateProgram();
    glAttachShader(ID , vertex);
    glAttachShader(ID , fragment);
    glLinkProgram(ID);
    
    glGetProgramiv(ID , GL_LINK_STATUS , &success);
    if(!success){
        glGetProgramInfoLog(ID , 512 , NULL , infoLog);
        std::cerr << "error in linking the program:\n" << infoLog << std::endl;
    }
    
    //delete the shaders after linking to the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}
void Shader::use(){
    glUseProgram(ID);
}
