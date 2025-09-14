#ifndef CAMERA
#define CAMERA
#include <openglPCH.hpp>
class Camera {
    public:
        bool doUpdateMouse;
        Camera(const glm::vec3 cameraPosition, const float pitch, const float yaw, const int windowWidth, const int windowHeight);
        void updateProjection(const int windowWidth, const int windowHeight);
        void moveCamera(const glm::vec3 relativeMovement);
        void updateMouse(const float mouseX, const float mouseY);
        glm::mat4 getViewProjection();
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraAngle;
        glm::mat4 view;
        glm::mat4 projection;

        float lastMouseX;
        float lastMouseY;
        float pitch;
        float yaw;
        void updateView();
};

#endif