#include <inputMethods.hpp>
#include <camera.hpp> //need camera API
#include <keyBindings.hpp>

bool mouseIsLocked = true;
GLFWcursor* arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Camera& camera = (static_cast<userPointers*>(glfwGetWindowUserPointer(window))->camera);
    camera.updateProjection(width, height);
}

void keypress_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        Camera& camera = (static_cast<userPointers*>(glfwGetWindowUserPointer(window))->camera);
        //itemSelected is a reference so it can be modified through the reference
        uint32_t& itemSelected = static_cast<userPointers*>(glfwGetWindowUserPointer(window))->itemSelected;
        if (key == Key::toggleMouse) {
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
        if (key == Key::placeBlock) {
            UniqueComponentSystem& system = static_cast<userPointers*>(glfwGetWindowUserPointer(window))->componentSystem;

            glm::vec3 cursorPosition = glm::round(camera.getPosition() + 3.0f * camera.getViewDirection());

            if (itemSelected != 0 && !(componentExists(glm::round(cursorPosition)))) {
                std::cout << "created component" << std::endl;

                //UPDATE TO USE ALL MESH ATTRIBUTES
                //system.createComponent(cursorPosition, , Color::darkRed, 1, LogicType::AND);
                const meshAttributes& attribs = Geometry::meshAttribArray[itemSelected];
                system.createComponent(glm::round(cursorPosition), attribs.startingMatrix, attribs.color, attribs.meshIndex, attribs.logic);
            }
            else {
                std::cout << "didn't create component" << std::endl;
            }
            //for creating unique component:
            /*glm::mat4 transform = glm::translate(identityMat4, cursorPosition);

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

            system.createUniqueComponent(&vertices[0], 36*3, LogicType::AND);*/
            
        }

        //Check if mode change
        for (unsigned int i = 0; i < numOptions; i++) {
            if (key == Key::selectItem[i]) { // 0-8 maps to 1-9
                itemSelected = i;
                std::cout << "selected " << i << std::endl;
            }
        }
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