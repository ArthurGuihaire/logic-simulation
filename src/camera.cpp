#include <camera.hpp>
#include <constants.hpp>

Camera::Camera(glm::vec3 startingPosition, float startingPitch, float startingYaw, int windowWidth, int windowHeight)
 : cameraPosition(startingPosition), pitch(startingPitch), yaw(startingYaw), doUpdateMouse(false)
{
    cameraAngle.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraAngle.y = sin(glm::radians(pitch));
    cameraAngle.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    view = glm::lookAt(cameraPosition, cameraPosition + cameraAngle, upVector);
    projection = glm::perspective(glm::radians(45.0f), (float) windowWidth / (float) windowHeight, 0.01f, 100.0f);
}

void Camera::updateProjection(const int windowWidth, const int windowHeight) {
    projection = glm::perspective(glm::radians(90.0f), (float) windowWidth / (float) windowHeight, 0.05f, 100.0f);
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

        if (pitch < 1.0f)
            pitch = 1.0f;
        else if (pitch > 179.0f)
            pitch = 179.0f;

        updateView();
    }
    else {
        doUpdateMouse = true;
    }
}