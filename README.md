# Voxel engine
This is an attempt at making a voxel engine using OpenGL and c++

![bilde](https://user-images.githubusercontent.com/72559835/157936591-7c8d9441-bd39-4935-88da-896ebc38854a.png)


## Build
> Note: Currently this code only supports windows

In order to build this you must have git, python and cmake installed. 
```batch
git clone https://github.com/KaderHA/VoxelEngine --recursive
cd VoxelEngine
mkdir build
cd build
cmake -G (Use the build tools you want) ..\
```
I used MinGW to compile but it works with Visual Studio too. You have to change the start up project to the Voxel Engine and
you have to swap the working directory to the build directory for Visual Studio.
