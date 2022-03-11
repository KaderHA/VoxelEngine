cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/VoxelEngine")

cmake_policy(SET CMP0048 NEW)
project(VoxelEngine)


add_subdirectory(VoxelEngine)