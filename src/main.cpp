#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <openglPCH.hpp>
#include <textureRenderer.hpp>
#include <initializer.hpp>
#include <constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <textureLoader.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    Initializer init = Initializer();
    init.setWindowSize(windowWidth, windowHeight);
    init.initGLFW();
    auto window = init.createWindow();
    init.initGLAD();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, windowWidth, windowHeight);

    std::cout << glGetString(GL_RENDERER) << "\n";
    std::cout << glGetString(GL_VERSION) << "\n";


    glm::mat4 model = identity_mat4;
    glm::mat4 view = glm::translate(identity_mat4, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);

    TexturedObject::init();

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

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set a non-black color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        test.render(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
