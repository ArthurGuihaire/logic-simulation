#include <inputMethods.hpp>

bool mouseIsLocked = true;
GLFWcursor* arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    //UpdateProjection width height
}

void keypress_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        if (mouseIsLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursor(window, arrowCursor);
            mouseIsLocked = false;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseIsLocked = true;
            //doUpdateMouse = false;
        }
    }
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
    //move camera
}

void cursor_position_callback(GLFWwindow* window, double x_position, double y_position) {
    if (mouseIsLocked) {}
        //Update mouse
}