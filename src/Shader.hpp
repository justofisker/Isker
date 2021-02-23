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
    void SetUniform1iv(const std::string &uniform, int *v, int count);
    void SetUniformMat4(const std::string &uniform, const glm::mat4 &mat);
private:
    int GetUniformLoc(const std::string &uniform);
};