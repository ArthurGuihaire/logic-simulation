#include <vertexBuffer.hpp>
#include <glad.h>
#include <utils.hpp>

VertexBuffer::VertexBuffer(const void* data, const unsigned int sizeBytes) {
    bufferSize = roundUpInt(sizeBytes, gpuBufferMultiple);
    usedMemory = sizeBytes;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeBytes, data);
}

VertexBuffer::VertexBuffer() {
    bufferSize = 1024;
    usedMemory = 0;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, gpuBufferMultiple, nullptr, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

unsigned int VertexBuffer::getUsedMemorySize() {
    return usedMemory;
}

unsigned int VertexBuffer::getBufferSize() {
    return bufferSize;
}

void VertexBuffer::UpdateData(const unsigned int offset, const void* data, const unsigned int sizeBytes) const {
    glBufferSubData(m_RendererID, offset, sizeBytes, data);
}

void VertexBuffer::AddData(const void* data, const unsigned int sizeBytes) {
    usedMemory += sizeBytes;
    glBufferSubData(m_RendererID, usedMemory, sizeBytes, data);
}

void VertexBuffer::RemoveData(const unsigned int sizeBytes) {
    usedMemory -= sizeBytes;
}

void VertexBuffer::UploadBuffer(const void* data, const unsigned int sizeBytes) {
    bufferSize = roundUpInt(sizeBytes, gpuBufferMultiple);
    usedMemory = sizeBytes;
    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeBytes, data);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind()const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}