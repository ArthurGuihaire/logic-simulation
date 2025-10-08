#include <utils.hpp>
#include <componentSystem.hpp>
#include <inputMethods.hpp>

int main() {
    GLFWwindow* window = initOpenGL();

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
