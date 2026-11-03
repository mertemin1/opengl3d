# opengl3d 🧊

**Developer:** Mert Emin İlhan  
**Repository:** <https://github.com/mertemin1/opengl3d>

This project is a C++17 application that leverages OpenGL, GLFW, and GLAD to load and render 3D models in the `.obj` format. It provides a custom-built 3D viewer capable of parsing and displaying models with vertex positions, normals, and texture coordinates without relying on third-party model-loading libraries.

## ✨ Features

* **Custom OBJ Model Loader:** Parses `.obj` files from scratch via command-line arguments (`argv[1]`) to extract 3D model data, including vertices, texture coordinates, and normals.
* **Modern 3D Rendering Pipeline:** Utilizes OpenGL 4.6 with GLAD for dynamic function loading, optimizing vertex data via Vertex Buffer Objects (VBO) and Element Buffer Objects (EBO).
* **Real-Time 3D Visualization:** Displays the loaded mesh in a GLFW window with dynamic aspect ratio scaling and continuous time-based model rotation (`glfwGetTime()`).
* **Custom Shader System:** Compiles and links GLSL vertex and fragment shaders directly from external source files.
* **Matrix Transformations:** Uses the GLM library for handling $4 \times 4$ matrices (Model, View, Projection) with a fixed perspective camera setup and dynamic model rotation.
* **Cross-Platform Build System:** Includes a custom `makefile` supporting both Linux (`X11`, `pthread`, `dl`) and Windows (`opengl32`, `gdi32`) environments.

## 🛠️ Tech Stack & Dependencies

* **Language:** C++17 (`g++` / `gcc`)
* **Graphics API:** OpenGL 4.6
* **Windowing & Context:** GLFW (`glfw3`)
* **OpenGL Loader:** GLAD (headers and implementation included in repository)
* **Math Library:** GLM (header-only library included in `includes/glm`)
* **Build Tool:** GNU Make (`makefile`)

## 🚀 Installation & Build

The repository already includes the required headers (`GLFW`, `glad`, `KHR`, `glm`, and `shader.hpp`) inside the `includes/` directory. To compile the project, ensure you have `g++`, `gcc`, `make`, and the system-level GLFW/OpenGL libraries installed.

```bash
# 1. Clone the repository
git clone https://github.com/mertemin1/opengl3d.git
cd opengl3d

# 2. Compile using the cross-platform Makefile
make

# 3. Clean object files and binaries (optional)
make clean
```

* **On Linux:** Generates the `program` executable linked with `-lglfw -lGL -ldl -lm -lX11 -lpthread`.
* **On Windows:** Generates the `program.exe` executable linked with `-lglfw3 -lopengl32 -lgdi32`.

## 📖 Usage

After building the project, run the compiled executable from the root directory by passing the path of a `.obj` model file as a command-line argument (`argc` / `argv[1]`):

```bash
# Linux
./program obj/<model_name>.obj

# Windows
./program.exe obj/<model_name>.obj
```

### In-Code Architecture

* **Context & Window Management:** `source/main.cpp` initializes GLFW and GLAD, configures the OpenGL context, and handles dynamic viewport updates via `glfwFrameBufferSizeCallback`.
* **OBJ Parsing & Buffer Optimization:** Implemented directly within `source/main.cpp` using `Vertex` and `ObjFile` structures. The `readObj` function extracts raw geometry data from the file passed via `argv[1]`, while `createVertices` eliminates duplicate vertices and prepares index buffers for efficient GPU rendering via VBO and EBO.
* **Shader Compilation:** The `Shader` class (`source/shader.cpp` and `includes/shader.hpp`) reads, compiles, and links `source/shaders/vertex.txt` and `source/shaders/fragment.txt`.
* **Render Loop:** Manages frame clearing, applies time-dependent Model rotation (`glfwGetTime()`) alongside fixed View and Projection ($MVP$) matrices using GLM, and executes indexed draw calls.

## 📁 Project Structure

```text
opengl3d/
├── README.md
├── makefile                 # Cross-platform build script (Linux & Windows)
├── includes/
│   ├── GLFW/                # GLFW headers
│   ├── glad/                # GLAD headers
│   ├── KHR/                 # KHR platform headers
│   ├── glm/                 # GLM math library headers
│   └── shader.hpp           # Shader class header
├── obj/                     # Sample .obj 3D models for testing
└── source/
    ├── shaders/
    │   ├── fragment.txt     # GLSL Fragment Shader
    │   └── vertex.txt       # GLSL Vertex Shader
    ├── glad.c               # GLAD implementation
    ├── main.cpp             # Main application logic, window setup & OBJ parser
    └── shader.cpp           # Shader class implementation
```

## 📄 License

This project is open-source and available under the MIT License.