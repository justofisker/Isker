#include "Shader.hpp"

#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <SDL2/SDL_log.h>

unsigned int Shader::s_CurrentlyBoundProgram = UINT32_MAX;

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
    : m_ProgramID(SDL_MAX_UINT32)
{
    auto read_file = [](const std::string &path) -> std::string {
        constexpr auto read_size = std::size_t{4096};
        auto stream = std::ifstream{path.data()};
        stream.exceptions(std::ios_base::badbit);

        auto out = std::string{};
        auto buf = std::string(read_size, '\0');
        while (stream.read(& buf[0], read_size)) {
            out.append(buf, 0, stream.gcount());
        }
        out.append(buf, 0, stream.gcount());
        return out;
    };

    std::string vertex_source = read_file(vertex_path);
    const char *vertex_cstr = vertex_source.c_str();
    const int vertex_length = (int)vertex_source.length();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_cstr, &vertex_length);
    glCompileShader(vertex_shader);

    GLint compile_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertex_shader, 1024 - 1, &log_length, message);
        SDL_Log("Vertex Shader (%s) failed to compile.\n%s\n", vertex_path.c_str(), message);

        glDeleteShader(vertex_shader);
        return;
    }

    std::string fragment_source = read_file(fragment_path);
    const char *fragment_cstr = fragment_source.c_str();
    const int fragment_length = (int)fragment_source.length();
    
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_cstr, &fragment_length);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragment_shader, 1024 - 1, &log_length, message);
        SDL_Log("Fragment Shader (%s) failed to compile.\n%s\n", fragment_path.c_str(), message);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024 - 1, &log_length, message);
        SDL_Log("Failed to link program (%s & %s)\n%s\n", vertex_path.c_str(), fragment_path.c_str(), message);
    }
    
    glDetachShader(program, vertex_shader);
    glDeleteShader(vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(fragment_shader);
    
    m_ProgramID = program;
}

Shader::~Shader()
{
    glDeleteProgram(m_ProgramID);
}

void Shader::Bind() const
{
    glUseProgram(m_ProgramID);
    s_CurrentlyBoundProgram = m_ProgramID;
}

bool Shader::IsValid() const
{
    return m_ProgramID != SDL_MAX_UINT32;
}

void Shader::SetFloat(const std::string &uniform, float v)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1f(loc, v);
}

void Shader::SetVec2(const std::string &uniform, const glm::vec2 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2f(loc, vec.x, vec.y);
}

void Shader::SetVec3(const std::string &uniform, const glm::vec3 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::SetVec4(const std::string &uniform, const glm::vec4 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetInt(const std::string &uniform, int v)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1i(loc, v);
}

void Shader::SetIVec2(const std::string &uniform, const glm::ivec2 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2i(loc, vec.x, vec.y);
}

void Shader::SetIVec3(const std::string &uniform, const glm::ivec3 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3i(loc, vec.x, vec.y, vec.z);
}

void Shader::SetIVec4(const std::string &uniform, const glm::ivec4 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetFloatArray(const std::string &uniform, int count, float *v)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1fv(loc, count, v);
}

void Shader::SetVec2Array(const std::string &uniform, int count, glm::vec2 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2fv(loc, count, (float*)vec);
}

void Shader::SetVec3Array(const std::string &uniform, int count, glm::vec3 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3fv(loc, count, (float*)vec);
}

void Shader::SetVec4Array(const std::string &uniform, int count, glm::vec4 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4fv(loc, count, (float*)vec);
}

void Shader::SetIntArray(const std::string &uniform, int count, int *v)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1iv(loc, count, v);
}

void Shader::SetIVec2Array(const std::string &uniform, int count, glm::ivec2 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2iv(loc, count, (int*)vec);
}

void Shader::SetIVec3Array(const std::string &uniform, int count, glm::ivec3 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3iv(loc, count, (int*)vec);
}

void Shader::SetIVec4Array(const std::string &uniform, int count, glm::ivec4 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4iv(loc, count, (int*)vec);
}

void Shader::SetMat2(const std::string &uniform, const glm::mat2 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &uniform, const glm::mat3 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &uniform, const glm::mat4 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat2Array(const std::string &uniform, int count, glm::mat2 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat3Array(const std::string &uniform, int count, glm::mat3 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat4Array(const std::string &uniform, int count, glm::mat4 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat2x3(const std::string &uniform, const glm::mat2x3 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3x2(const std::string &uniform, const glm::mat3x2 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat2x4(const std::string &uniform, const glm::mat2x4 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4x2(const std::string &uniform, const glm::mat4x2 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3x4(const std::string &uniform, const glm::mat3x4 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4x3(const std::string &uniform, const glm::mat4x3 &mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat2x3Array(const std::string &uniform, int count, glm::mat2x3 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x3fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat3x2Array(const std::string &uniform, int count, glm::mat3x2 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x2fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat2x4Array(const std::string &uniform, int count, glm::mat2x4 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x4fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat4x2Array(const std::string &uniform, int count, glm::mat4x2 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x2fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat3x4Array(const std::string &uniform, int count, glm::mat3x4 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x4fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetMat4x3Array(const std::string &uniform, int count, glm::mat4x3 *mat)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x3fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUnsignedInt(const std::string &uniform, unsigned int v)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1ui(loc, v);
}

void Shader::SetUVec2(const std::string &uniform, const glm::uvec2 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2ui(loc, vec.x, vec.y);
}

void Shader::SetUVec(const std::string &uniform, const glm::uvec3 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3ui(loc, vec.x, vec.y, vec.z);
}

void Shader::SetUVec4(const std::string &uniform, const glm::uvec4 &vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4ui(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUnsignedIntArray(const std::string &uniform, int count, unsigned int *v)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1uiv(loc, count, v);
}

void Shader::SetUVec2Array(const std::string &uniform, int count, glm::uvec2 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2uiv(loc, count, (unsigned int*)vec);
}

void Shader::SetUVec3Array(const std::string &uniform, int count, glm::uvec3 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3uiv(loc, count, (unsigned int*)vec);
}

void Shader::SetUVec4Array(const std::string &uniform, int count, glm::uvec4 *vec)
{
    if(s_CurrentlyBoundProgram != m_ProgramID)
    {
        glUseProgram(m_ProgramID);
        s_CurrentlyBoundProgram = m_ProgramID;
        SDL_Log("Avoid setting a uniform for a shader that is not currently bound.\n");
    }
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4uiv(loc, count, (unsigned int*)vec);
}

int Shader::GetUniformLoc(const std::string &uniform)
{
    if(m_Uniforms.find(uniform) == m_Uniforms.end())
    {
        int loc = glGetUniformLocation(m_ProgramID, uniform.c_str());
        if(loc == -1)
        {
            SDL_Log("Unable to find uniform %s for shader %d.\n", uniform.c_str(), m_ProgramID);
            return - 1;
        }
        m_Uniforms[uniform] = loc;
        return loc;
    }
    return m_Uniforms[uniform];
}
