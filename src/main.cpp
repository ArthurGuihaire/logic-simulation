#include <GLFW/glfw3.h>
#include <openglPCH.hpp>
#include <textureRenderer.hpp>
#include <initializer.hpp>
#include <constants.hpp>
#include <textureLoader.hpp>

bool mouseIsLocked = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    TexturedObject::updateProjection(width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W))
        movement.z += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S))
        movement.z -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A))
        movement.x -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D))
        movement.x += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        movement.y += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        movement.y -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_E)) {
        if (mouseIsLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseIsLocked = false;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseIsLocked = true;
        }
    }
    TexturedObject::moveCamera(movement);
}

void cursor_position_callback(GLFWwindow* window, double x_position, double y_position) {
    TexturedObject::updateMouse(x_position, y_position);
}

int main() {
    Initializer init = Initializer();
    init.setWindowSize(windowWidth, windowHeight);
    init.initGLFW();
    auto window = init.createWindow();
    init.initGLAD();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glEnable(GL_DEPTH_TEST);

    std::cout << glGetString(GL_RENDERER) << "\n";
    std::cout << glGetString(GL_VERSION) << "\n";

    TexturedObject::init(windowWidth, windowHeight);

    float vertices[12] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    unsigned int tuxTextureId = loadTexture("textures/linux_penguin.jpg");
    TexturedObject test(vertices, tuxTextureId);

    std::cout << "Created texture object" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        test.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
