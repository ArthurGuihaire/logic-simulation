#include <gpuBuffer.hpp>
#include <glad.h>
#include <utils.hpp>
#include <constants.hpp>

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

gpuBuffer::gpuBuffer()
 : m_bufferSize(1024), m_usedMemory(0)
{
    glGenBuffers(1, &m_RendererID);
}

gpuBuffer::~gpuBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void gpuBuffer::createBuffer(const unsigned int bufferType) {
    m_bufferType = bufferType;
    glBindBuffer(m_bufferType, m_RendererID);
    glBufferData(m_bufferType, gpuBufferMultiple, nullptr, GL_STATIC_DRAW);
}

void gpuBuffer::createBuffer(const unsigned int bufferType, const void* data, const unsigned int sizeBytes) {
    m_bufferType = bufferType;
    m_bufferSize = roundUpInt(sizeBytes, gpuBufferMultiple);
    m_usedMemory = sizeBytes;
    glBufferData(m_bufferType, m_bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(m_bufferType, 0, sizeBytes, data);
}

unsigned int gpuBuffer::getUsedMemorySize() {
    return m_usedMemory;
}

unsigned int gpuBuffer::getBufferSize() {
    return m_bufferSize;
}

void gpuBuffer::updateData(const unsigned int offsetBytes, const void* data, const unsigned int sizeBytes) const {
    glBufferSubData(m_RendererID, offsetBytes, sizeBytes, data);
}

void gpuBuffer::addData(const void* data, const unsigned int sizeBytes) {
    m_usedMemory += sizeBytes;
    glBufferSubData(m_RendererID, m_usedMemory, sizeBytes, data);
}

void gpuBuffer::removeData(const unsigned int sizeBytes) {
    m_usedMemory -= sizeBytes;
}

void gpuBuffer::uploadBuffer(const void* data, const unsigned int sizeBytes) {
    m_bufferSize = roundUpInt(sizeBytes, gpuBufferMultiple);
    m_usedMemory = sizeBytes;
    glBufferData(m_bufferType, m_bufferSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(m_bufferType, 0, sizeBytes, data);
}

void gpuBuffer::bind() const {
    glBindBuffer(m_bufferType, m_RendererID);
}

void gpuBuffer::unbind()const {
    glBindBuffer(m_bufferType, 0);
}