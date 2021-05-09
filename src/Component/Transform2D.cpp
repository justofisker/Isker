#include "Transform2D.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../Render/Renderer.hpp"

Transform2D::Transform2D(const glm::vec2 &_translation, const glm::vec2 &_scale, float _rotation)
        : m_Translation(_translation), m_Scale(_scale), m_Rotation(_rotation), m_DoRecalculateMatrix(true), m_Matrix(glm::mat4(1.0f)) { }

const glm::mat4 &Transform2D::GetMatrix()
{
    if(m_DoRecalculateMatrix)
    {
        RecalculateMatrix();
        m_DoRecalculateMatrix = false;
    }
    return m_Matrix;
}

void Transform2D::SetTranslation(const glm::vec2 translation)
{
    if(translation != m_Translation)
    {
        m_Translation = translation;
        m_DoRecalculateMatrix = true;
    }
}

void Transform2D::SetScale(const glm::vec2 scale)
{
    if(scale != m_Scale)
    {
        m_Scale = scale;
        m_DoRecalculateMatrix = true;
    }
}

void Transform2D::SetRotation(float rotation)
{
    if(rotation != m_Rotation)
    {
        m_Rotation = rotation;
        m_DoRecalculateMatrix = true;
    }
}

void Transform2D::RecalculateMatrix()
{
    m_Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_Translation.x, -m_Translation.y + Renderer::Get().GetGameSize().y, 0.0f));
    m_Matrix = glm::scale(m_Matrix, glm::vec3(m_Scale, 1.0f));
    m_Matrix = glm::rotate(m_Matrix, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}
