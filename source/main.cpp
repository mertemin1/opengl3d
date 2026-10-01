
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 ourColor;

    void main(){
        gl_Position = vec4(aPos, 1.0f);
        ourColor = aColor;
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 330 core
    
    out vec4 fragColor;
    in vec3 ourColor; 
    uniform float greenColor;

    void main(){
        fragColor = vec4(ourColor.x , greenColor , ourColor.y , 1.0f);
    }
    )";


//function that changes viewport when window size gets changed
void framebufferSizeCallback(GLFWwindow * window , int width , int height){
        glViewport(0 , 0 , width , height);
    }

//input handler
void handleInput(GLFWwindow* window){
    if( glfwGetKey(window , GLFW_KEY_ESCAPE) == GLFW_PRESS ){
        glfwSetWindowShouldClose(window , true);
    }
}

void errorCallback(int error ,const char* desc){
    std::cout << "error : " << error << ":" << desc << std::endl;
}
int main(){

    //pass the error catcher func to glfw
    glfwSetErrorCallback(errorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800 , 600 , "Sea" , NULL , NULL);
    if (window == NULL){
        std::cout << "failed to create a glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << " failed to init glad" << std::endl;
        return -1;
    }

    //tell the opengl the size of window
    glViewport(0 , 0 , 800 , 600);

    //create a function so that when window size gets changed, adjust the viewport
    

    //pass that function to glfw
    glfwSetFramebufferSizeCallback(window , framebufferSizeCallback);


    //compile the shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader , 1 , &vertexShaderSource , NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram , vertexShader);
    glAttachShader(shaderProgram , fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);
    

    //triangle data
    float vertices [] = {
        0.5f , 0.5f , 0.0f , 1.0f , 0.0f , 0.0f,
        -0.5f , 0.5f , 0.0f, 0.0f , 0.0f ,1.0f,
        0.5f , -0.5f , 0.0f, 0.0f , 1.0f , 0.0f,
        -0.5f , -0.5f , 0.0f, 0.0f , 0.0f , 0.0f
    };
    
    unsigned int indices [] = {
        0 , 1 , 2,
        1 , 2 , 3
    };
    //init vao and vbo

    unsigned int VBO;
    glGenBuffers(1 , &VBO);
    
    unsigned int EBO;
    glGenBuffers(1 , &EBO);
    

    unsigned int VAO;
    glGenVertexArrays(1 , &VAO);
    
    
    //bind the vao fırst
    glBindVertexArray(VAO);

    //bind vbo
    glBindBuffer(GL_ARRAY_BUFFER , VBO );
    glBufferData(GL_ARRAY_BUFFER , sizeof(vertices) , vertices , GL_STATIC_DRAW);
    
    //bind veo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER ,  sizeof(indices) , indices , GL_STATIC_DRAW);


    //declare the attr pointer
    // 0 = index of location data in vertex shader( layout (location = 0) )
    // 3 = number of elements
    //GL_FLOAT = data type
    //GL_FALSE = asks us if we wanna clamp the data between -1 and +1 (our data is already between these two)
    //3 * sizeof(float) = step length, how many bytes between data of each corner
    //(void *) 0 = beggining of the data
    //position
    glVertexAttribPointer(0 ,3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , (void*)0 );
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);


    //render loop
    while(!glfwWindowShouldClose(window)){

        handleInput(window);

        //clear the screen first
        glClearColor(0.2f , 0.2f , 0.3f , 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        //activate shader
        glUseProgram(shaderProgram);
        
        float timeValue = glfwGetTime();
        float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram , "greenColor");
        glUniform1f(vertexColorLocation , greenValue);
        
        //loads both the settings of vbo and ebo at the same time, render the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0);

        
        
        
        
        //pass the buffer to screen , check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //terminate program

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}