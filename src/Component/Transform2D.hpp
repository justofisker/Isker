#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

class Transform2D
{
private:
    glm::vec2 m_Translation;
    glm::vec2 m_Scale;
    float m_Rotation;
    glm::mat4 m_Matrix;
    bool m_DoRecalculateMatrix;
public:
    Transform2D(const glm::vec2 &translation = glm::vec2(0.0f), const glm::vec2 &scale = glm::vec2(1.0f), float rotation = 0.0f);
    inline const glm::vec2 &GetTranslation() const { return m_Translation; }
    inline const glm::vec2 &GetScale() const { return m_Scale; }
    inline float GetRotation() const { return m_Rotation; }
    const glm::mat4 &GetMatrix();
    void SetTranslation(const glm::vec2 translation);
    void SetScale(const glm::vec2 scale);
    void SetRotation(float rotation);
    operator const glm::mat4&()
    {
        if(m_DoRecalculateMatrix)
        {
            RecalculateMatrix();
            m_DoRecalculateMatrix = false;
        }
        return m_Matrix;
    }
private:
    void RecalculateMatrix();
};