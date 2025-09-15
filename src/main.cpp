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
    init.initGLFW();
    auto window = init.createWindow();
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
    //Debug output
    /*glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* msg, const void*) {
        if (severity == GL_DEBUG_SEVERITY_HIGH) fprintf(stderr, "GL ERROR: %s\n", msg);
    }, nullptr);*/

    //Print driver info
    std::cout << glGetString(GL_RENDERER) << "\n";
    std::cout << glGetString(GL_VERSION) << "\n";

    Camera camera(glm::vec3(-3.0f, 0.0f, 0.0f), 90.0f, 0.0f, windowWidth, windowHeight);
    Renderer renderer(camera);
    ComponentSystem componentSystem(renderer);

    userPointers callbackUtils = {camera, componentSystem};

    glfwSetWindowUserPointer(window, &callbackUtils); //Make camera accessible inside callback functions

    componentSystem.createComponent(&Geometry::cubeVertices[0], sizeof(Geometry::cubeVertices) / sizeof(float), LogicType::AND);

    while (!glfwWindowShouldClose(window)) {
        std::cout << "Still running 1" << std::endl;
        processInput(window);
        std::cout << "Still running 2" << std::endl;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        std::cout << "Still running 3" << std::endl;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::cout << "Still running 4" << std::endl;

        //componentSystem.printIndirectDraw(shaderType::Ethereal, 0);
        renderer.renderFrame();
        std::cout << "Still running 5" << std::endl;

        glfwSwapBuffers(window);
        std::cout << "Still running 6" << std::endl;
        glfwPollEvents();
        std::cout << "Still running 7" << std::endl;
    }
}
