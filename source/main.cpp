
#include <iostream>
#include <cmath>
#include "shader.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <string.h>
#include <unordered_map>
#include <glm/gtx/hash.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

typedef struct Vertex{
    glm::vec3 position;
    glm::vec3 norm;
    glm::vec2 textureCoords;
} Vertex;    

typedef struct vertexIndex{ 
    unsigned int vIndex;
    unsigned int vtIndex; 
    unsigned int vnIndex;

    bool operator== (const vertexIndex other) const{
        return vIndex == other.vIndex && vtIndex == other.vtIndex && vnIndex == other.vnIndex;
    };


} vertexIndex;


typedef struct ObjFile{
    //vertices
    std::vector<glm::vec3> v;
    //vertice texture coordinates
    std::vector<glm::vec2> vt;
    //vertice norm
    std::vector<glm::vec3> vn;
    //title
    std::string g;
    //indices for triangles
    std::vector<vertexIndex> f;
} ObjFile;    



void glfwErrorCallback(int error , const char* desc){ 
    std::cerr<< error << " : " << desc << std::endl; 
}    

void glfwFrameBufferSizeCallback(GLFWwindow* window , int width , int height){
    glViewport(0 , 0 , width , height);
}    

//placeholder for testing , delete after implementing obj parser
const char* objfile = R"(
# Blender Triangulated Cube    
v 1.000000 1.000000 -1.000000
v 1.000000 -1.000000 -1.000000
v 1.000000 1.000000 1.000000
v 1.000000 -1.000000 1.000000
v -1.000000 1.000000 -1.000000
v -1.000000 -1.000000 -1.000000
v -1.000000 1.000000 1.000000
v -1.000000 -1.000000 1.000000
vt 0.625000 0.500000
vt 0.875000 0.500000
vt 0.875000 0.750000
vt 0.625000 0.750000
vt 0.375000 0.750000
vt 0.625000 0.000000
vt 0.375000 1.000000
vt 0.375000 0.000000
vt 0.625000 0.250000
vt 0.125000 0.500000
vt 0.375000 0.500000
vt 0.125000 0.750000
vt 0.375000 0.250000
vt 0.625000 1.000000
vn 0.0000 1.0000 0.0000
vn 0.0000 0.0000 1.0000
vn -1.0000 0.0000 0.0000
vn 0.0000 -1.0000 0.0000
vn 1.0000 0.0000 0.0000
vn 0.0000 0.0000 -1.0000

#bunu kendim ekledim hata verirse satiri silmeyi dene
g cube

s off
f 1/1/1 5/2/1 7/3/1
f 1/1/1 7/3/1 3/4/1
f 4/5/2 3/4/2 7/6/2
f 4/5/2 7/6/2 8/7/2
f 8/8/3 7/9/3 5/10/3
f 8/8/3 5/10/3 6/11/3
f 6/12/4 2/13/4 4/5/4
f 6/12/4 4/5/4 8/14/4
f 2/13/5 1/1/5 3/4/5
f 2/13/5 3/4/5 4/5/5
f 6/11/6 5/10/6 1/1/6
f 6/11/6 1/1/6 2/13/6
)";


void readObj(ObjFile &file){
    std::istringstream fileStream(objfile);
    std::string line;
    
    //divide the obj text into lines
    while(std::getline(fileStream , line)){
        if(line.empty() || line[0] == '#') continue;

        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if(prefix == "v"){
            glm::vec3 v;
            lineStream >> v.x >> v.y >> v.z;
            file.v.push_back(v);
        }
        else if(prefix == "vt"){
            glm::vec2 v;
            lineStream >> v.x >> v.y;
            file.vt.push_back(v);
        }
        else if(prefix == "vn"){
            glm::vec3 v;
            lineStream >> v.x >> v.y >> v.z;
            file.vn.push_back(v);
        }
        else if(prefix == "g"){
            lineStream >> file.g;
        }
        else if (prefix == "f"){
            std::string s;
            std::vector<vertexIndex> indices; 
            while (lineStream >> s){
                vertexIndex vi;
                unsigned int matches = std::sscanf(s.c_str() , "%d/%d/%d" ,&vi.vIndex , &vi.vtIndex , &vi.vnIndex );
                
                //obj indexes start from 1 
                if(matches > 1) vi.vIndex -1;
                if(matches > 2) vi.vtIndex -1;
                if(matches > 3) vi.vnIndex -1;
                indices.push_back(vi);
                if(indices.size() ==  3){
                    file.f.push_back(indices[0]);
                    file.f.push_back(indices[1]);
                    file.f.push_back(indices[2]);
                }
                //if the f values aren't for triangles , turn them into triangles
                else if(indices.size() == 4){
                    //1st triangle 0 1 2
                    file.f.push_back(indices[0]);
                    file.f.push_back(indices[1]);
                    file.f.push_back(indices[2]);
                    
                    //2nd triangle 0 2 3 
                    file.f.push_back(indices[0]);
                    file.f.push_back(indices[2]);
                    file.f.push_back(indices[3]);

                }
            }
            
        }

        
    }
}
//for debugging, delete later
void printObj(const ObjFile &file){
    std::cout << "Group: " << file.g << '\n';

    std::cout << "Vertices (" << file.v.size() << "):\n";
    for(const auto &v : file.v)
        std::cout << "  " << v.x << ' ' << v.y << ' ' << v.z << '\n';

    std::cout << "Texture coordinates (" << file.vt.size() << "):\n";
    for(const auto &vt : file.vt)
        std::cout << "  " << vt.x << ' ' << vt.y << '\n';

    std::cout << "Normals (" << file.vn.size() << "):\n";
    for(const auto &vn : file.vn)
        std::cout << "  " << vn.x << ' ' << vn.y << ' ' << vn.z << '\n';

    std::cout << "Face indices (" << file.f.size() << "):\n";
    for(const auto &face : file.f)
        std::cout << "  " << face.vIndex << '/' << face.vtIndex << '/' << face.vnIndex << '\n';
}

void createVertices(ObjFile& file){
    Vertex v; 
    
    std::vector<unsigned int> ebo;
    //there may be different vertices in a single coordinate. change glm::vec3 to vertexIndex and make changes accordingly
    std::unordered_map<glm::vec3 , unsigned int> dict;
    int counter = 0;
    
    for(int i = 0 ; i < file.f.size() ; i++){
        //create the vertex
        v.position = file.v[file.f[i].vIndex];
        v.norm = file.vn[file.f[i].vnIndex];
        v.textureCoords = file.vt[file.f[i].vtIndex];
        
        //add to the hash map
        auto [it , isAdded] = dict.try_emplace(v.position , counter);
        if(isAdded){
            ebo.push_back(counter);
            counter++;
        }
        else{
            ebo.push_back(it->second);
        }
    }

}





int main(){

    ObjFile obj;
    readObj(obj);
    printObj(obj);
    
    glfwSetErrorCallback(glfwErrorCallback);
    
    //init window
    if(!glfwInit()){
        std::cerr << "init failed" << std::endl;
        return -1;
    }

    //create the window 
    GLFWwindow* window = glfwCreateWindow(600 , 800 , "Obj Viewer" , NULL , NULL);
    if(!window){
        std::cerr << "window could not be created" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    //init glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress )){
        std::cerr << "failed to init glad" << std::endl;
        return -1;
    }
    glViewport(0 , 0 , 800 , 600);
    glfwSetFramebufferSizeCallback(window , glfwFrameBufferSizeCallback);
    
    //compile and use the shaders
    Shader shaderProgram("source/shaders/vertex.txt" , "source/shaders/fragment.txt");
    shaderProgram.use(); 


    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    
    //init buffers
    unsigned int VBO , EBO , VAO;
    glGenVertexArrays(1 , &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind vao first
    //other bindings and configs in bvo and ebos will be saved into this vao
    glBindVertexArray(VAO);

    //bind and upload vertex data (vbo)
    glBindBuffer(GL_ARRAY_BUFFER , VBO);
    glBufferData(GL_ARRAY_BUFFER , vertices.size() * sizeof(Vertex), &vertices[0] , GL_STATIC_DRAW);

    // bind and upload index data (ebo)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER , sizeof(indices)*sizeof(float) , &indices[0] , GL_STATIC_DRAW);

    //tell opengl how to parse VBO data
    //location 0 -> position
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_TRUE , 3*(sizeof(float)) , (void*) 0 );
    glEnableVertexAttribArray(0);



    

    while(!glfwWindowShouldClose(window)){
        //keep running
        glClearColor(0.2f, 0.2f , 0.5f , 1.0f);
        shaderProgram.use();
        glBindVertexArray(VAO);

    }


    //terminate glfw
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}