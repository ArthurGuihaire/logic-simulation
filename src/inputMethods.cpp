#include <cstring> // For std::memcpy
#include <GLFW/glfw3.h>
#include <constants.hpp>
#include <inputMethods.hpp>
#include <camera.hpp> //need camera API
#include <keyBindings.hpp>

bool mouseIsLocked = true;
GLFWcursor* arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Camera& camera = (static_cast<userPointers*>(glfwGetWindowUserPointer(window))->camera);
    camera.updateProjection(windowWidth, windowHeight);
}

void keypress_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Camera& camera = (static_cast<userPointers*>(glfwGetWindowUserPointer(window))->camera);
    if (key == Key::toggleMouse && action == GLFW_PRESS) {
        camera.doUpdateMouse = !camera.doUpdateMouse;
        if (mouseIsLocked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursor(window, arrowCursor);
            mouseIsLocked = false;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseIsLocked = true;
        }
    }
    if (key == Key::placeBlock && action == GLFW_PRESS) {
        UniqueComponentSystem& system = static_cast<userPointers*>(glfwGetWindowUserPointer(window))->componentSystem;

        glm::vec3 camPosition = camera.getPosition() + 3.0f * camera.getViewDirection();
        glm::mat4 transform = glm::translate(identity_mat4, camPosition);

        constexpr size_t vertexCount = sizeof(BadGeometry::cube) / (3 * sizeof(float));
        float vertices[vertexCount * 3];

        for (uint32_t i = 0; i < vertexCount * 3; i += 3) {
            glm::vec4 transformed = transform * glm::vec4(
                BadGeometry::cube[i + 0],
                BadGeometry::cube[i + 1],
                BadGeometry::cube[i + 2],
                1.0f
            );

            vertices[i + 0] = transformed.x;
            vertices[i + 1] = transformed.y;
            vertices[i + 2] = transformed.z;
        }

        system.createUniqueComponent(&vertices[0], 36*3, LogicType::AND);
        std::cout << "Created component maybe" << std::endl;
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, Key::forward))
        movement.z += movementSpeed;
    if (glfwGetKey(window, Key::backward))
        movement.z -= movementSpeed;
    if (glfwGetKey(window, Key::left))
        movement.x -= movementSpeed;
    if (glfwGetKey(window, Key::right))
        movement.x += movementSpeed;
    if (glfwGetKey(window, Key::up))
        movement.y += movementSpeed;
    if (glfwGetKey(window, Key::down))
        movement.y -= movementSpeed;
    Camera& camera = (static_cast<userPointers*>(glfwGetWindowUserPointer(window))->camera);
    camera.moveCamera(movement);
}

void cursor_position_callback(GLFWwindow* window, double x_position, double y_position) {
    if (mouseIsLocked) {}
        Camera& camera = (static_cast<userPointers*>(glfwGetWindowUserPointer(window))->camera);
        camera.updateMouse(x_position, y_position);
}