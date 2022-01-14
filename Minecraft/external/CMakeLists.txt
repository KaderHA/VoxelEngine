add_subdirectory(GLFW)
add_subdirectory(GLAD)
add_subdirectory(GLM)
add_library(SimplexNoise  "${CMAKE_CURRENT_SOURCE_DIR}/SimplexNoise/src/SimplexNoise.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/SimplexNoise/src/SimplexNoise.h")
target_include_directories(SimplexNoise PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/SimplexNoise/src")

add_library(external INTERFACE)
target_link_libraries(external INTERFACE glad glfw glm SimplexNoise)
