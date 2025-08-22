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
        static unsigned int uniformViewLocation;
        static unsigned int uniformProjectionLocation;
        static unsigned int uniformTextureLocation;
        glm::mat4 model;
        uint16_t rotationDegrees[3];
    public:
        TexturedObject(float startingVertices[12], unsigned int textureID);
        void render(glm::mat4 view, glm::mat4 projection);
        static void init();
};