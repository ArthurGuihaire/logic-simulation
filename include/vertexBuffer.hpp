#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER
class VertexBuffer {
    private:
        unsigned int m_RendererID;
    public:
        VertexBuffer(const void* data, unsigned int size);
        VertexBuffer();
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;
};
#endif