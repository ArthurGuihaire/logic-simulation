#include <initializer.hpp>
#include <iostream>
#include <gl.h>

Initializer::Initializer() {}

void Initializer::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

void Initializer::initGLFW(int majorVersion, int minorVersion) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* Initializer::createWindow(bool fullscreen) {
    GLFWwindow* window;
    if (fullscreen) {
        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidMode = glfwGetVideoMode(mon);
        window = glfwCreateWindow(vidMode->width, vidMode->height, "OpenGL", mon, NULL);
    }
    else {
        window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
    }
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
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, windowWidth, windowHeight);
}