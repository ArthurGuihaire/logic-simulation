#include <gpuBuffer.hpp>
#include <glad.h>
#include <utils.hpp>

gpuBuffer::gpuBuffer(const void* data, const unsigned int sizeBytes, const unsigned int bufferType)
 : m_bufferSize(bufferType), m_usedMemory(sizeBytes), m_bufferType(bufferType)
{
    m_bufferSize = roundUpInt(sizeBytes, gpuBufferMultiple);
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(m_bufferType, m_RendererID);
    glBufferData(m_bufferType, m_bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(m_bufferType, 0, sizeBytes, data);
}

gpuBuffer::gpuBuffer(const unsigned int bufferType)
 : m_bufferSize(1024), m_usedMemory(0), m_bufferType(bufferType)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(m_bufferType, m_RendererID);
    glBufferData(m_bufferType, gpuBufferMultiple, nullptr, GL_STATIC_DRAW);
}

gpuBuffer::~gpuBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

unsigned int gpuBuffer::getUsedMemorySize() {
    return m_usedMemory;
}

unsigned int gpuBuffer::getBufferSize() {
    return m_bufferSize;
}

void gpuBuffer::UpdateData(const unsigned int offset, const void* data, const unsigned int sizeBytes) const {
    glBufferSubData(m_RendererID, offset, sizeBytes, data);
}

void gpuBuffer::AddData(const void* data, const unsigned int sizeBytes) {
    m_usedMemory += sizeBytes;
    glBufferSubData(m_RendererID, m_usedMemory, sizeBytes, data);
}

void gpuBuffer::RemoveData(const unsigned int sizeBytes) {
    m_usedMemory -= sizeBytes;
}

void gpuBuffer::UploadBuffer(const void* data, const unsigned int sizeBytes) {
    m_bufferSize = roundUpInt(sizeBytes, gpuBufferMultiple);
    m_usedMemory = sizeBytes;
    glBufferData(m_bufferType, m_bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(m_bufferType, 0, sizeBytes, data);
}

void gpuBuffer::Bind() const {
    glBindBuffer(m_bufferType, m_RendererID);
}

void gpuBuffer::Unbind()const {
    glBindBuffer(m_bufferType, 0);
}