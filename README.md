# In The Middle Of Nowhere

### Brief Description 

This is a 3D game using OpenGL with C++. It's a primitive game created for Computer Graphics Course, where I only use simple 3d models and created shaders for some effects using OpenGL. It's about an explorer, in the middle of a forsaken forest (or farm) with some old houses in it, is trying to find an ancient key. He is all alone there at the middle of the night, where nothing but moon light guides him.

### Utilized Technologies and Frameworks

- C++ OpenGL Libraies, such as: GLFW, GLAD and GLM.
- C++ tinyobjloader.
- CMake.

### Repository Structure

**1) assets:**
- Contains the 3d models, textures and implemented shaders for rendering and effects.

**2) common:**
- Contains the main classes of the program, such as: framebuffers, controllers, mesh drawing, ...etc.

**3) src:**
- Contains the class of the main scene, where everything is put together and the main entry point of the program.

**4) vendor:**
- Contains the sources of some libraries used in the program.

**5) CMakeLists.txt:**
- The text file for building the game using CMake.

### Implemented Shaders

- Texture Mapping.
- Sky Texture, using Volume Texture Mapping.
- Grain Noise.
- Shadow Mapping, using Depth Maps.

### Implemented Shaders

1) Make sure your graphics drivers are installed.
2) Install OpenGL on your machine.
3) Run CMakeLists.txt file using CMake, along with preferred C++ Compiler.

Have a nice day!
