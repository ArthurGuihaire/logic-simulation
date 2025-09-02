#ifndef INDEX_BUFFER
#define INDEX_BUFFER
const static inline unsigned int gpuBufferMultiple = 1024;
class IndexBuffer {
    private:
        unsigned int m_RendererID;
        unsigned int bufferSize;
        unsigned int usedMemory;
    public:
        IndexBuffer(const void* data, const unsigned int sizeBytes);
        IndexBuffer();
        ~IndexBuffer();
        unsigned int getUsedMemorySize();
        unsigned int getBufferSize();
        void UpdateData(const unsigned int offset, const void* data, const unsigned int sizeBytes) const;
        void AddData(const void* data, const unsigned int sizeBytes);
        void RemoveData(const unsigned int sizeBytes);
        void UploadBuffer(const void* data, const unsigned int sizeBytes);
        void Bind() const;
        void Unbind() const;
};
#endif