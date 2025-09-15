#include <camera.hpp>
#include <constants.hpp>

Camera::Camera(glm::vec3 startingPosition, float startingPitch, float startingYaw, int windowWidth, int windowHeight)
 : doUpdateMouse(true), cameraPosition(startingPosition), pitch(startingPitch), yaw(startingYaw)
{
    cameraAngle.x = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraAngle.y = cos(glm::radians(pitch));
    cameraAngle.z = sin(glm::radians(pitch)) * sin(glm::radians(yaw));

    view = glm::lookAt(cameraPosition, cameraPosition + cameraAngle, upVector);
    projection = glm::perspective(glm::radians(90.0f), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);
    //std::cout << cameraAngle.x << ", " << cameraAngle.y << ", " << cameraAngle.z << std::endl;
}

void Camera::updateProjection(const int windowWidth, const int windowHeight) {
    projection = glm::perspective(glm::radians(90.0f), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);
}

void Camera::updateView() {
    cameraAngle.x = sin(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraAngle.y = cos(glm::radians(pitch));
    cameraAngle.z = sin(glm::radians(pitch)) * sin(glm::radians(yaw));

    view = glm::lookAt(cameraPosition, cameraPosition + cameraAngle, upVector);
}

void Camera::moveCamera(const glm::vec3 relativeMovement) {
    cameraPosition += glm::normalize(glm::vec3(cameraAngle.x, 0.0f, cameraAngle.z)) * relativeMovement.z;
    cameraPosition += glm::vec3(0.0f, relativeMovement.y, 0.0f);
    if (relativeMovement.x != 0.0f) {
        glm::vec3 cameraAngleCross = glm::normalize(glm::cross(cameraAngle, upVector));
        cameraPosition += cameraAngleCross * relativeMovement.x;
    }

    updateView();
}

void Camera::updateMouse(const float mouseX, const float mouseY) {
    const float xOffset = mouseX - lastMouseX;
    const float yOffset = mouseY - lastMouseY; // y is inverted since pitch=0 is up
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    if (doUpdateMouse) {
        pitch += yOffset * sensitivity;
        yaw += xOffset * sensitivity;

        if (pitch > 179.0f) pitch = 179.0f;
        else if (pitch < 1.0f) pitch = 1.0f;


        updateView();
    }
    /*else {
        doUpdateMouse = true;
    }*/
}

glm::mat4 Camera::getViewProjection() {
    return (projection * view);
}

glm::vec3& Camera::getPosition() {
    return cameraPosition;
}

glm::vec3& Camera::getViewDirection() {
    return cameraAngle;
}
