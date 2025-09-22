#include <utils.hpp>
#include <GLFW/glfw3.h>
#include <openglPCH.hpp>
#include <initializer.hpp>
#include <constants.hpp>
#include <textureLoader.hpp>
#include <componentSystem.hpp>
#include <inputMethods.hpp>

int main() {
    Initializer init = Initializer();
    init.setWindowSize(windowWidth, windowHeight);
    init.initGLFW(3, 3);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    auto window = init.createWindow(false);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    init.initGLAD();
    glViewport(0, 0, windowWidth, windowHeight);

    //Set functions for resize, mouse, and keyboard
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, keypress_callback);

    //Set mouse mode and style to default mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Enable OpenGL depth testing
    glEnable(GL_DEPTH_TEST);
    
    //Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Debug output
    glfwSetErrorCallback(glfwErrorCallback);
    /*glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    std::cout << "before callback" << std::endl;
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) { std::cerr << "[GL DEBUG] " <<  message << std::endl; }, nullptr);
    std::cout << "after callback" << std::endl;*/

    //Print driver info
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    Camera camera(glm::vec3(-3.0f, 0.0f, 0.0f), 90.0f, 0.0f, windowWidth, windowHeight);
    Renderer renderer(camera);
    UniqueComponentSystem componentSystem(renderer);

    userPointers callbackUtils = {camera, componentSystem, 0};

    glfwSetWindowUserPointer(window, &callbackUtils); //Make camera accessible inside callback functions

    //auto start = std::chrono::high_resolution_clock::now();
    
    componentSystem.createComponent({0.0f, 0.0f, 0.0f}, identityMat4, Color::darkGreen, 1, LogicType::AND);

    /*auto end = std::chrono::high_resolution_clock::now();

    // compute duration
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";*/

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.renderFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}   
