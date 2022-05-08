# Light Field Particle System
The goal of this thesis is to propose and implement a method combining light field and
particle effects. Particles are rendered as billboards and their textures are created with
synthetic light field, which is dynamically generated at runtime. Particles can also use
several of these textures at the same time. The thesis also includes several implemented
scenes demonstrating various functionality and measurements comparing proposed method
with standard 3D rendering techniques.

Simple demo video can be found [here](https://www.youtube.com/watch?v=jVIBj0W4zsU) or in the *video* folder

```
+---application         Implemented application
|   +---assets          Assets, this folder is copied to build folder
|   |   +---imgs        Images, for skyboxes
|   |   +---models      Models for generating light fields
|   |   +---shaders     Containts all used shader programs
|   +---include         Libs and submodules
|   +---scripts         Scripts for benchmarking, graphs, etc.
|   +---src             Source files
+---text                Thesis text in PDF
|   +---source          Tex source files for PDF
+---video               Simple demo video
```

# Setup
## Repository
This thesis is also avaiable on [Github](https://github.com/JanSkvaril/LightFieldParticleSystem). You can clone this repository by running: 
``` 
git clone --recursive https://github.com/JanSkvaril/LightFieldParticleSystem.git
```
`--recursive` flag is required, because repository contains two submodules
## Compile
For compilation you must have cmake installed. Program was compilled by `Visual Studio Community 2022` compiler on windows and `GPP` on linux. At least OpenGL 4.X must be installed and it must supported by your GPU. 

### Windows
Majority of development of this thesis was done in Visual Studio 2022 with C++ extention. 

In application folder:
```
mkdir build
cd build
```
Then run cmake
```
cmake ..
```
And build the application
```
cmake --build . --target light_field_ps_bp --config Release --parallel 12
```
and run the application
```
.\Release\light_field_ps_bp.exe
```
You can also use visual studio to run the solution
### Linux
Before compilation, install necessary dependencies:
```
# Build essentials:
    sudo apt-get install -y build-essential
# NanoGUI:
    sudo apt-get install -y cmake xorg-dev libglu1-mesa-dev
# OpenGL:
    sudo apt-get install -y mesa-common-dev mesa-utils freeglut3-dev 
```
In application folder:
```
mkdir build
cd build
```
Then run cmake
```
cmake ..
```
And build the application
```
cmake --build . --parallel 12
```
and run the application
```
./light_field_ps_bp 
```
