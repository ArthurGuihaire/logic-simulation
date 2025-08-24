cmake_minimum_required(VERSION 3.16)
project(logicSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set debug symbols
#set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wunused)
elseif (MSVC)
    add_compile_options(/W4)
endif()

cmake_policy(SET CMP0072 NEW)  # Prefer modern OpenGL behavior

# Define source and include directories
set(SOURCE_DIR  "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Gather C++ source files (excluding glad.c)
file(GLOB SRC_FILES "${SOURCE_DIR}/*.cpp")
file(GLOB COMMON_CPP "${SOURCE_DIR}/common/*.cpp")

# Create a static library for glad.c (C-only)
add_library(glad STATIC "${SOURCE_DIR}/common/glad.c")
target_include_directories(glad PUBLIC ${INCLUDE_DIR})

# Create the executable with only C++ files
add_executable(${PROJECT_NAME} ${SRC_FILES} ${COMMON_CPP})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Add precompiled headers (applies only to this target)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR}/openglPCH.hpp)

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad)

# Copy shaders and textures directories to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR})