# Raycasting Demo
Raycasting game engine

![Screenshot](https://preview.ibb.co/huVc7n/Screen_Shot_2018_03_04_at_17_27_02.png "Screenshot")

## How to build
This project should work on Windows, Linux and Mac OS. Tested on Mac OS and Windows

1. Build (or get) [static GLFW library](http://www.glfw.org/) for your system. Instructions on how to compile can be found [here](http://www.glfw.org/docs/latest/compile_guide.html)
2. Visit [this](http://glad.dav1d.de/) site to get latest glad.h and glad.c files. Make sure to pick OpenGL 3.3 Core and C++ language

   * You'll have to replace __glad.h__ in the *include/* directory on your file
   
   * Do not forget to replace __glad.c__ file in the *src/* directory

3. Create a project in the IDE of your choise (ex. __XCode__ or __Visual Studio__) and add all files from *src/* directory.
4. Link library and include *include* directory from this repository
5. Compile the program and put contents of the __resources__ directory near executable. Or they should be put in the working directory of your IDE.
6. Run the program and have fun :)

* Note for Windows users - link opengl32.lib library as well
* Note for XCode users - include the following frameworks
  * CoreVideo
  * IOKit
  * Cocoa
  * OpenGL
