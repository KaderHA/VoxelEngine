add_subdirectory(GLFW)
add_subdirectory(GLAD)
add_subdirectory(GLM)

add_library(external INTERFACE)
target_link_libraries(external INTERFACE glad glfw glm)
