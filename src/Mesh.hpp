#pragma once

class Mesh
{
public:
    const unsigned int m_VAO, m_VertexBuffer, m_IndexBuffer;
public:
    Mesh(unsigned int VAO, unsigned int VB, unsigned int IB)
        : m_VAO(VAO), m_VertexBuffer(VB), m_IndexBuffer(IB) {}
};