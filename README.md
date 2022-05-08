# Light Field Particle System
kfdjdkfd

# Setup
## Repository
This thesis is also avaiable on github: link. You can clone this repository by running: 
``` 
git clone --recursive git@github.com:JanSkvaril/lightfield_particle_system.git
```
## Compile
For compilation you must have cmake installed. Program was compilled by `Visual Studio Community 2022` compiler on windows and `GPP` on linux. At least OpenGL 4.X must be installed and it must supported by your GPU. 

### Windows
Majority of development of this thesis was done in Visual Studio 2022 with C++ extention. 

In repository folder:
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
In repository folder:
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
