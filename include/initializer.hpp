#ifndef INITIALIZER
#define INITIALIZER
#include <GLFW/glfw3.h>

class Initializer {
    private:
        int windowWidth;
        int windowHeight;
    public:
        Initializer();
        void setWindowSize(int width, int height);
        void initGLFW();
        void initGLAD();
        GLFWwindow* createWindow();
};
#endif