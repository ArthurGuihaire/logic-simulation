#include <glm/glm.hpp>
#include <memory>
#include <vertexBuffer.hpp>
#include <indexBuffer.hpp>

class TexturedObject {
    private:
        float vertices[20];
        unsigned int texture;
        static unsigned int shader;
        unsigned int vao;
        std::unique_ptr<VertexBuffer> vb;
        std::unique_ptr<IndexBuffer> ib;
        static unsigned int uniformModelLocation;
        static unsigned int uniformProjectionViewLocation;
        static unsigned int uniformTextureLocation;
        glm::mat4 model;
        static glm::mat4 view;
        static glm::mat4 projection;
        uint16_t rotationDegrees[3];

        static float lastMouseX;
        static float lastMouseY;
    public:
        TexturedObject(float (&startingVertices)[12], unsigned int textureID);
        void render();
        static void init(const int width, const int height);
        static void updateProjection(const int width, const int height);
        static void updateView(const glm::mat4 viewUpdateMatrix);
        static void updateMouse(const float mouseX, const float mouseY);
};