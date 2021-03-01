#pragma once

#include <string>
#include <map>

#include <glm/glm.hpp>

class Shader
{
private:
    unsigned int m_ProgramID;
    std::map<std::string, unsigned int> m_Uniforms;
public:
    Shader(const std::string &path_vertex, const std::string &path_fragment);
    ~Shader();
    void Bind();

    void SetUniform1f(const std::string &uniform, float v);
    void SetUniform2f(const std::string &uniform, const glm::vec2 &vec);
    void SetUniform3f(const std::string &uniform, const glm::vec3 &vec);
    void SetUniform4f(const std::string &uniform, const glm::vec4 &vec);
    void SetUniform1i(const std::string &uniform, int v);
    void SetUniform2i(const std::string &uniform, const glm::ivec2 &vec);
    void SetUniform3i(const std::string &uniform, const glm::ivec3 &vec);
    void SetUniform4i(const std::string &uniform, const glm::ivec4 &vec);
    void SetUniform1fv(const std::string &uniform, int count, float *v);
    void SetUniform2fv(const std::string &uniform, int count, glm::vec2 *vec);
    void SetUniform3fv(const std::string &uniform, int count, glm::vec3 *vec);
    void SetUniform4fv(const std::string &uniform, int count, glm::vec4 *vec);
    void SetUniform1iv(const std::string &uniform, int count, int *v);
    void SetUniform2iv(const std::string &uniform, int count, glm::ivec2 *vec);
    void SetUniform3iv(const std::string &uniform, int count, glm::ivec3 *vec);
    void SetUniform4iv(const std::string &uniform, int count, glm::ivec4 *vec);
    void SetUniformMatrix2(const std::string &uniform, const glm::mat2 &mat);
    void SetUniformMatrix3(const std::string &uniform, const glm::mat3 &mat);
    void SetUniformMatrix4(const std::string &uniform, const glm::mat4 &mat);
    void SetUniformMatrix2v(const std::string &uniform, int count, glm::mat2 *mat);
    void SetUniformMatrix3v(const std::string &uniform, int count, glm::mat3 *mat);
    void SetUniformMatrix4v(const std::string &uniform, int count, glm::mat4 *mat);
    void SetUniformMatrix2x3(const std::string &uniform, const glm::mat2x3 &mat);
    void SetUniformMatrix3x2(const std::string &uniform, const glm::mat3x2 &mat);
    void SetUniformMatrix2x4(const std::string &uniform, const glm::mat2x4 &mat);
    void SetUniformMatrix4x2(const std::string &uniform, const glm::mat4x2 &mat);
    void SetUniformMatrix3x4(const std::string &uniform, const glm::mat3x4 &mat);
    void SetUniformMatrix4x3(const std::string &uniform, const glm::mat4x3 &mat);
    void SetUniformMatrix2x3v(const std::string &uniform, int count, glm::mat2x3 *mat);
    void SetUniformMatrix3x2v(const std::string &uniform, int count, glm::mat3x2 *mat);
    void SetUniformMatrix2x4v(const std::string &uniform, int count, glm::mat2x4 *mat);
    void SetUniformMatrix4x2v(const std::string &uniform, int count, glm::mat4x2 *mat);
    void SetUniformMatrix3x4v(const std::string &uniform, int count, glm::mat3x4 *mat);
    void SetUniformMatrix4x3v(const std::string &uniform, int count, glm::mat4x3 *mat);
    void SetUniform1ui(const std::string &uniform, unsigned int v);
    void SetUniform2ui(const std::string &uniform, const glm::uvec2 &vec);
    void SetUniform3ui(const std::string &uniform, const glm::uvec3 &vec);
    void SetUniform4ui(const std::string &uniform, const glm::uvec4 &vec);
    void SetUniform1uiv(const std::string &uniform, int count, unsigned int *v);
    void SetUniform2uiv(const std::string &uniform, int count, glm::uvec2 *vec);
    void SetUniform3uiv(const std::string &uniform, int count, glm::uvec3 *vec);
    void SetUniform4uiv(const std::string &uniform, int count, glm::uvec4 *vec);

private:
    int GetUniformLoc(const std::string &uniform);
};