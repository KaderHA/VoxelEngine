cmake_minimum_required(VERSION 2.22.0)

cmake_policy(SET CMP0048 NEW)
project(Minecraft)

# set(MYVAR "hello world!")

# function(print)
#     foreach(var ${ARGN})
#         message("${var} = ${${var}}")
#     endforeach()
# endfunction()

# print(PROJECT_NAME PROJECT_NAME_FOO MYVAR)

add_subdirectory(Minecraft)