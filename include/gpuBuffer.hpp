#ifndef GPU_BUFFER
#define GPU_BUFFER

class gpuBuffer {
    public:
        gpuBuffer(const void* data, const unsigned int sizeBytes, const unsigned int bufferType);
        gpuBuffer(const unsigned int bufferType);
        gpuBuffer();
        ~gpuBuffer();
        unsigned int getUsedMemorySize();
        unsigned int getBufferSize();
        void UpdateData(const unsigned int offset, const void* data, const unsigned int sizeBytes) const;
        void AddData(const void* data, const unsigned int sizeBytes);
        void RemoveData(const unsigned int sizeBytes);
        void UploadBuffer(const void* data, const unsigned int sizeBytes);
        void Bind() const;
        void Unbind() const;
    private:
        constexpr static inline unsigned int gpuBufferMultiple = 1024;
        unsigned int m_RendererID;
        unsigned int m_bufferSize;
        unsigned int m_usedMemory;
        const unsigned int m_bufferType;
};
#endif