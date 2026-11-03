# opengl3d 🧊

This project is a C++ application that leverages OpenGL, GLFW, and GLAD to load and render 3D models in the `.obj` format. It provides a basic 3D viewer capable of displaying models with vertex positions, normals, and texture coordinates.

## Badges 🏆

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## ✨ Features

-   **OBJ Model Loading:** Parses `.obj` files to load 3D model data, including vertices, texture coordinates, and normals.
-   **3D Rendering:** Utilizes OpenGL with GLAD for modern OpenGL function loading to render the loaded 3D models.
-   **Interactive Viewer:** Displays the 3D model in a GLFW window with basic camera controls and rotation.
-   **Shader Integration:** Implements a shader system for rendering, loading vertex and fragment shaders from text files.
-   **Matrix Transformations:** Uses the GLM library for handling 4x4 matrices (model, view, projection) for transformations and camera setup.

## 🛠️ Tech Stack

-   **Language:** C++
-   **Graphics API:** OpenGL (version 4.6 supported by GLAD)
-   **Windowing & Context:** GLFW
-   **OpenGL Loader:** GLAD
-   **Math Library:** GLM (OpenGL Mathematics)

## 🚀 Installation

To build and run this project, you will need a C++ compiler and the following libraries:

1.  **GLFW:** For window creation and input handling.
2.  **GLAD:** For loading OpenGL function pointers.
3.  **GLM:** For mathematical operations (vectors, matrices).

**Build Steps (Example using CMake - adjust as needed):

```bash
# Clone the repository
git clone https://github.com/mertemin1/opengl3d.git
cd opengl3d

# Create a build directory
mkdir build
cd build

# Configure with CMake (assuming libraries are installed and discoverable)
cmake ..

# Build the project
cmake --build .
```

**Note:** The provided `makefile` is present but its content was not fully analyzed. Ensure your build environment is set up correctly with the necessary dependencies.

## 📖 Usage

This project serves as a 3D model viewer, specifically designed to load and render `.obj` files.

### Running the Application

After successfully building the project, you can run the executable from the `build` directory by providing the path to an `.obj` file as a command-line argument:

```bash
./program <path/to/your/model.obj>
```

**Example:**

If you have a model file named `teapot.obj` in the root directory, you would run:

```bash
./program ../teapot.obj
```

**Note:** The executable is named `program` in the root directory, and the source code for the main application is in `source/main.cpp`.

### In-Code Functionality

-   The `main.cpp` file initializes GLFW and GLAD, sets up the OpenGL context, and creates a window.
-   It defines `Vertex` and `ObjFile` structures to handle model data.
-   The `readObj` function parses the `.obj` file, extracting vertex, texture coordinate, and normal data.
-   The `createVertices` function processes the parsed data, optimizes it for rendering (removing duplicate vertices), and prepares it for VBO and EBO.
-   Shaders (vertex and fragment) are compiled and linked using the `Shader` class, loaded from `source/shaders/vertex.txt` and `source/shaders/fragment.txt`.
-   The main loop handles rendering, clearing the screen, applying model, view, and projection transformations using GLM, and drawing the loaded mesh.
-   The camera's aspect ratio is dynamically updated via `glfwFrameBufferSizeCallback` when the window is resized.

## 📁 Project Structure

```
opengl3d/
├── README.md
├── build/
├── includes/
│   ├── GLFW/        # GLFW headers (assumed)
│   ├── glad/        # GLAD headers
│   ├── KHR/         # KHR platform headers
│   └── glm/         # GLM math library headers
├── makefile
├── program        # Compiled executable
├── source/
│   ├── shaders/
│   │   ├── fragment.txt
│   │   └── vertex.txt
│   ├── glad.c       # GLAD implementation
│   ├── main.cpp     # Main application logic
│   ├── objloader.cpp # OBJ loading implementation (empty file)
│   ├── objloader.hpp # OBJ loading header
│   ├── shader.cpp   # Shader class implementation
│   ├── shader.hpp   # Shader class header
│   └── window.cpp   # Window creation implementation
│   └── window.hpp   # Window creation header
└── YOUR_MODEL.obj   # Example OBJ file (not included in repo)
```

## 📚 Dependencies

-   **GLFW:** Essential for window management and OpenGL context creation.
-   **GLAD:** Required for loading OpenGL function pointers, enabling compatibility with different OpenGL versions.
-   **GLM:** A header-only C++ mathematics library for vectors and matrices, crucial for 3D graphics transformations.

## 🤝 Contributing

As this project appears to be a personal learning exercise or a small utility, formal contribution guidelines are not provided. However, if you wish to contribute, consider:

-   **Reporting Bugs:** If you encounter any issues, please open an issue on the GitHub repository.
-   **Suggesting Improvements:** Ideas for new features or enhancements are welcome.
-   **Submitting Pull Requests:** For code contributions, please ensure your code is well-commented and follows the existing coding style.

## 📄 License

This project is likely intended for educational purposes or personal use. Based on common practices for such projects, it might be released under a permissive license. If a specific license file is present, please refer to it. Assuming a common open-source license, a placeholder is provided:

*This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details (if available).* 

*(Note: No explicit license file was found in the provided analysis. The MIT license is a common choice for such projects.)*

## 🔗 Important Links

-   **Project Repository:** [https://github.com/mertemin1/opengl3d](https://github.com/mertemin1/opengl3d)
-   **OpenGL Documentation:** [https://www.opengl.org/documentation/](https://www.opengl.org/documentation/)
-   **GLFW Official Website:** [https://www.glfw.org/](https://www.glfw.org/)
-   **GLAD GitHub:** [https://github.com/Dav1dde/glad](https://github.com/Dav1dde/glad)
-   **GLM GitHub:** [https://github.com/g-truc/glm](https://github.com/g-truc/glm)

---

© 2023 [mertemin1]

This project is a demonstration of OpenGL 3D model rendering. We encourage you to explore, fork, star, and contribute to make it even better!

*   **Fork:** [Fork this repository](https://github.com/mertemin1/opengl3d/fork)
*   **Star:** [Star this repository](https://github.com/mertemin1/opengl3d/stargazers)
*   **Issues:** [Report Issues](https://github.com/mertemin1/opengl3d/issues)


---
**<p align="center">Generated by [ReadmeCodeGen](https://www.readmecodegen.com/)</p>**