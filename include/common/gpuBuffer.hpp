#ifndef GPU_BUFFER
#define GPU_BUFFER

class gpuBuffer {
    public:
        gpuBuffer(const void* data, const unsigned int sizeBytes, const unsigned int bufferType);
        gpuBuffer(const unsigned int bufferType);
        gpuBuffer();
        ~gpuBuffer();
        void createBuffer(const unsigned int bufferType);
        void createBuffer(const unsigned int bufferType, const void* data, const unsigned int sizeBytes);
        unsigned int getUsedMemorySize();
        unsigned int getBufferSize();
        void updateData(const unsigned int offsetBytes, const void* data, const unsigned int sizeBytes) const;
        void addData(const void* data, const unsigned int sizeBytes);
        void removeData(const unsigned int sizeBytes);
        void uploadBuffer(const void* data, const unsigned int sizeBytes);
        void bind() const;
        void unbind() const;
    private:
        constexpr static inline unsigned int gpuBufferMultiple = 4096;
        unsigned int m_RendererID;
        unsigned int m_bufferSize;
        unsigned int m_usedMemory;
        unsigned int m_bufferType;
};
#endif