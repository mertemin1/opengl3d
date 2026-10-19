
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
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

typedef struct Vertex{
    glm::vec3 position;
    glm::vec3 norm;
    glm::vec2 textureCoords;

    //need to define == operator overload for hashing , default does the job
    bool operator==(const Vertex& other) const {
        return position == other.position &&
        norm == other.norm && 
        textureCoords == other.textureCoords;

    }

} Vertex;    


//used to get the data from the f value on obj files
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

//add a hash template for vertex
template <>
struct std::hash<Vertex>{
     size_t operator()(const Vertex& v) const noexcept{ 
        std::hash<glm::vec3> hash3;
        std::hash<glm::vec2> hash2;

        size_t h1 = hash3(v.position);
        size_t h2 = hash3(v.norm);
        size_t h3 = hash2(v.textureCoords);

        size_t seed = 0;
        //industry standard for hash combining I guess, also that number is the golden ratio idfk 
        seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        
        return seed;
    
    }
};



void glfwErrorCallback(int error , const char* desc){ 
    std::cerr<< error << " : " << desc << std::endl; 
}    

//gets called when the window dimentions change
void glfwFrameBufferSizeCallback(GLFWwindow* window , int width , int height ){
    glViewport(0 , 0 , width , height);
    Shader::aspectRatio = (float)width/ (float)height;
    
    Shader* activeShader = (Shader*)glfwGetWindowUserPointer(window);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), Shader::aspectRatio, 0.1f, 100.0f);
    activeShader->use();
    
    //we get the address of the projection matrice of our shader in the gpu
    int projLoc = glGetUniformLocation(activeShader -> ID , "projection");
    //then update that value with our new projection variable
    glUniformMatrix4fv(projLoc , 1  , GL_FALSE , glm::value_ptr(projection) );

}    

//placeholder for testing , delete after implementing obj parser
const char* objfile = R"(
# Blender Triangulated Cube    
v 0.500000 0.500000 -0.500000
v 0.500000 -0.500000 -0.500000
v 0.500000 0.500000 0.500000
v 0.500000 -0.500000 0.500000
v -0.500000 0.500000 -0.500000
v -0.500000 -0.500000 -0.500000
v -0.500000 0.500000 0.500000
v -0.500000 -0.500000 0.500000
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
                
                
                if(matches > 0) --vi.vIndex;
                if(matches > 1) --vi.vtIndex;
                if(matches > 2) --vi.vnIndex;
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

void createVertices(ObjFile& file, std::vector<Vertex>& vertices, std::vector<unsigned int>& ebo){
    Vertex v; 
    std::unordered_map<Vertex, unsigned int> dict;
    
    for(auto i : file.f){
        
        //create the vertex
        v.position = file.v[i.vIndex];        
        v.norm = file.vn[i.vnIndex];
        v.textureCoords = file.vt[i.vtIndex];
        
        
        
        //add to the hash map
        //check if the vertex has already been added inside the ebo to avoid drawing the same vertex twice
        auto [it, isAdded] = dict.try_emplace(v, static_cast<unsigned int>(vertices.size()));
        if(isAdded){
            vertices.push_back(v);
            ebo.push_back(it->second);
        }
        else{
            ebo.push_back(it->second);
        }
    }

}

void testCreateVertices(std::vector<Vertex>& vbo, std::vector<unsigned int>& ebo){



    std::cout << "VBO (" << vbo.size() << "):\n";
    for(const auto& vertex : vbo){
        std::cout << "  position: " << vertex.position.x << ' ' << vertex.position.y << ' '
                  << vertex.position.z << ", normal: " << vertex.norm.x << ' '
                  << vertex.norm.y << ' ' << vertex.norm.z << ", texture: "
                  << vertex.textureCoords.x << ' ' << vertex.textureCoords.y << '\n';
    }

    std::cout << "EBO (" << ebo.size() << "):\n  ";
    for(size_t i = 0; i < ebo.size(); ++i){
        if(i > 0) std::cout << ' ';
        std::cout << ebo[i];
    }
    std::cout << '\n';
}





int main(){
    std::vector<Vertex> vbo;
    std::vector<unsigned int>ebo;
    ObjFile obj;
    readObj(obj);
    printObj(obj);
    createVertices(obj, vbo, ebo);
    testCreateVertices(vbo , ebo);
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
    
    //compile and use the shaders
    Shader shaderProgram("source/shaders/vertex.txt" , "source/shaders/fragment.txt");
    shaderProgram.use(); 
    
    //add the framebuffercallback the shader object address to be able to access it from the callback function
    glfwSetWindowUserPointer(window , &shaderProgram);
    
    glfwSetFramebufferSizeCallback(window , glfwFrameBufferSizeCallback);

   

    
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
    glBufferData(GL_ARRAY_BUFFER , vbo.size() * sizeof(Vertex), vbo.data() , GL_STATIC_DRAW);

    // bind and upload index data (ebo)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER , ebo.size() * sizeof(unsigned int) , ebo.data() , GL_STATIC_DRAW);

    //tell opengl how to parse VBO data
    //location 0 -> position
    glVertexAttribPointer(0 , 3 , GL_FLOAT ,GL_FALSE , (sizeof(Vertex)) , (void*) 0 );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.2f , 0.5f , 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.use();

        //matrices

        //location and the rotation of the ball
        glm::mat4 model = glm::mat4(1.0f);
        //third parameter defines the axis for the model to rotate, imagine it like a imaginary stick that goes through the model
        model = glm::rotate(model , (float)glfwGetTime() * glm::radians(30.0f) , glm::vec3( 1.0f , 0.5f , 0.0f));

        //camera matrix
        //since there are 16 values in a 4x4 matrix, glm gives us a translate method that puts the x y z values in the correct places
        glm::mat4 view = glm::mat4 (1.0f);
        view = glm::translate(view , glm::vec3(0.0f , 0.0f , -3.0f));

        //projection matrice, handles the perspective
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), Shader::aspectRatio, 0.1f, 100.0f);


        //find the location of the uniform in the shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        // send the matrice
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES , ebo.size() , GL_UNSIGNED_INT , 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //terminate glfw
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}