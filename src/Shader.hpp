#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader
{
private:
    unsigned int m_ProgramID;
    std::unordered_map<std::string, unsigned int> m_Uniforms;
    static unsigned int s_CurrentlyBoundProgram;
public:
    Shader(const std::string &path_vertex, const std::string &path_fragment);
    ~Shader();
    void Bind() const;
    bool IsValid() const;

    void SetFloat(const std::string &uniform, float v);
    void SetVec2(const std::string &uniform, const glm::vec2 &vec);
    void SetVec3(const std::string &uniform, const glm::vec3 &vec);
    void SetVec4(const std::string &uniform, const glm::vec4 &vec);
    void SetInt(const std::string &uniform, int v);
    void SetIVec2(const std::string &uniform, const glm::ivec2 &vec);
    void SetIVec3(const std::string &uniform, const glm::ivec3 &vec);
    void SetIVec4(const std::string &uniform, const glm::ivec4 &vec);
    void SetFloatArray(const std::string &uniform, int count, float *v);
    void SetVec2Array(const std::string &uniform, int count, glm::vec2 *vec);
    void SetVec3Array(const std::string &uniform, int count, glm::vec3 *vec);
    void SetVec4Array(const std::string &uniform, int count, glm::vec4 *vec);
    void SetIntArray(const std::string &uniform, int count, int *v);
    void SetIVec2Array(const std::string &uniform, int count, glm::ivec2 *vec);
    void SetIVec3Array(const std::string &uniform, int count, glm::ivec3 *vec);
    void SetIVec4Array(const std::string &uniform, int count, glm::ivec4 *vec);
    void SetMat2(const std::string &uniform, const glm::mat2 &mat);
    void SetMat3(const std::string &uniform, const glm::mat3 &mat);
    void SetMat4(const std::string &uniform, const glm::mat4 &mat);
    void SetMat2Array(const std::string &uniform, int count, glm::mat2 *mat);
    void SetMat3Array(const std::string &uniform, int count, glm::mat3 *mat);
    void SetMat4Array(const std::string &uniform, int count, glm::mat4 *mat);
    void SetMat2x3(const std::string &uniform, const glm::mat2x3 &mat);
    void SetMat3x2(const std::string &uniform, const glm::mat3x2 &mat);
    void SetMat2x4(const std::string &uniform, const glm::mat2x4 &mat);
    void SetMat4x2(const std::string &uniform, const glm::mat4x2 &mat);
    void SetMat3x4(const std::string &uniform, const glm::mat3x4 &mat);
    void SetMat4x3(const std::string &uniform, const glm::mat4x3 &mat);
    void SetMat2x3Array(const std::string &uniform, int count, glm::mat2x3 *mat);
    void SetMat3x2Array(const std::string &uniform, int count, glm::mat3x2 *mat);
    void SetMat2x4Array(const std::string &uniform, int count, glm::mat2x4 *mat);
    void SetMat4x2Array(const std::string &uniform, int count, glm::mat4x2 *mat);
    void SetMat3x4Array(const std::string &uniform, int count, glm::mat3x4 *mat);
    void SetMat4x3Array(const std::string &uniform, int count, glm::mat4x3 *mat);
    void SetUnsignedInt(const std::string &uniform, unsigned int v);
    void SetUVec2(const std::string &uniform, const glm::uvec2 &vec);
    void SetUVec(const std::string &uniform, const glm::uvec3 &vec);
    void SetUVec4(const std::string &uniform, const glm::uvec4 &vec);
    void SetUnsignedIntArray(const std::string &uniform, int count, unsigned int *v);
    void SetUVec2Array(const std::string &uniform, int count, glm::uvec2 *vec);
    void SetUVec3Array(const std::string &uniform, int count, glm::uvec3 *vec);
    void SetUVec4Array(const std::string &uniform, int count, glm::uvec4 *vec);
private:
    int GetUniformLoc(const std::string &uniform);
};