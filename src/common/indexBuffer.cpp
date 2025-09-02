#include <indexBuffer.hpp>
#include <glad.h>
#include <utils.hpp>

IndexBuffer::IndexBuffer(const void* data, const unsigned int sizeBytes) {
    bufferSize = roundUpUInt(sizeBytes, gpuBufferMultiple);
    usedMemory = sizeBytes;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeBytes, data);
}

IndexBuffer::IndexBuffer() {
    bufferSize = 1024;
    usedMemory = 0;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gpuBufferMultiple, nullptr, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

unsigned int IndexBuffer::getUsedMemorySize() {
    return usedMemory;
}

unsigned int IndexBuffer::getBufferSize() {
    return bufferSize;
}

void IndexBuffer::UpdateData(const unsigned int offset, const void* data, const unsigned int sizeBytes) const {
    glBufferSubData(m_RendererID, offset, sizeBytes, data);
}

void IndexBuffer::AddData(const void* data, const unsigned int sizeBytes) {
    usedMemory += sizeBytes;
    glBufferSubData(m_RendererID, usedMemory, sizeBytes, data);
}

void IndexBuffer::RemoveData(const unsigned int sizeBytes) {
    usedMemory -= sizeBytes;
}

void IndexBuffer::UploadBuffer(const void* data, const unsigned int sizeBytes) {
    bufferSize = roundUpUInt(sizeBytes, gpuBufferMultiple);
    usedMemory = sizeBytes;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeBytes, data);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind()const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}