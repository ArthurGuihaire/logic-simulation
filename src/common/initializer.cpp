#include <initializer.hpp>
#include <iostream>
#include "glad.h"

Initializer::Initializer() {}

void Initializer::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

void Initializer::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* Initializer::createWindow() {
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    //Create an OpenGL contex for the window
    glfwSwapInterval(1); //Limit framerate
    glfwMakeContextCurrent(window);
    return window;
}

void Initializer::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, windowWidth, windowHeight);
}