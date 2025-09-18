#pragma once
#include <GLFW/glfw3.h>

class Initializer {
    private:
        int windowWidth;
        int windowHeight;
    public:
        Initializer();
        void setWindowSize(int width, int height);
        void initGLFW(int majorVersion, int minorVersion);
        void initGLAD();
        GLFWwindow* createWindow();
};
