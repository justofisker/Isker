#include "Shader.hpp"

#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <SDL2/SDL_log.h>

static const char* GetFileContents(const char *path, int *length)
{
    FILE* file;
    file = fopen(path, "rb");
    if(!file)
    {
        printf("Failed to open %s", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    if(length)
        *length = fsize;
    fseek(file, 0, SEEK_SET);
    char* content = (char*)malloc(fsize + 1);
    fread(content, 1, fsize, file);
    fclose(file);
    content[fsize] = 0;
    return content;
}

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
{
    int vertex_source_length;
    const char* vertex_source = GetFileContents(vertex_path.c_str(), &vertex_source_length);
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, &vertex_source_length);
    glCompileShader(vertex_shader);
    free((void*)vertex_source);
    GLint vertex_compiled;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
    if (vertex_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertex_shader, 1024 - 1, &log_length, message);
        SDL_Log("Vertex Shader (%s) failed to compile.\n%s\n", vertex_path.c_str(), message);
    }
    int fragment_source_length;
    const char* fragment_source = GetFileContents(fragment_path.c_str(), &fragment_source_length);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, &fragment_source_length);
    glCompileShader(fragment_shader);
    free((void*)fragment_source);
    GLint fragment_compiled;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);
    if (fragment_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragment_shader, 1024 - 1, &log_length, message);
        SDL_Log("Fragment Shader (%s) failed to compile.\n%s\n", fragment_path.c_str(), message);
    }
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    GLint program_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024 - 1, &log_length, message);
        SDL_Log("Failed to link program (%s & %s)\n%s\n", vertex_path.c_str(), fragment_path.c_str(), message);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    m_ProgramID = program;
}

Shader::~Shader()
{
    glDeleteProgram(m_ProgramID);
}

void Shader::Bind()
{
    glUseProgram(m_ProgramID);
}

void Shader::SetUniform1f(const std::string &uniform, float v)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1f(loc, v);
}

void Shader::SetUniform2f(const std::string &uniform, const glm::vec2 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2f(loc, vec.x, vec.y);
}

void Shader::SetUniform3f(const std::string &uniform, const glm::vec3 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::SetUniform4f(const std::string &uniform, const glm::vec4 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform1i(const std::string &uniform, int v)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1i(loc, v);
}

void Shader::SetUniform2i(const std::string &uniform, const glm::ivec2 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2i(loc, vec.x, vec.y);
}

void Shader::SetUniform3i(const std::string &uniform, const glm::ivec3 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3i(loc, vec.x, vec.y, vec.z);
}

void Shader::SetUniform4i(const std::string &uniform, const glm::ivec4 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform1fv(const std::string &uniform, int count, float *v)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1fv(loc, count, v);
}

void Shader::SetUniform2fv(const std::string &uniform, int count, glm::vec2 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2fv(loc, count, (float*)vec);
}

void Shader::SetUniform3fv(const std::string &uniform, int count, glm::vec3 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3fv(loc, count, (float*)vec);
}

void Shader::SetUniform4fv(const std::string &uniform, int count, glm::vec4 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4fv(loc, count, (float*)vec);
}

void Shader::SetUniform1iv(const std::string &uniform, int count, int *v)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1iv(loc, count, v);
}

void Shader::SetUniform2iv(const std::string &uniform, int count, glm::ivec2 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2iv(loc, count, (int*)vec);
}

void Shader::SetUniform3iv(const std::string &uniform, int count, glm::ivec3 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3iv(loc, count, (int*)vec);
}

void Shader::SetUniform4iv(const std::string &uniform, int count, glm::ivec4 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4iv(loc, count, (int*)vec);
}

void Shader::SetUniformMatrix2(const std::string &uniform, const glm::mat2 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix3(const std::string &uniform, const glm::mat3 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix4(const std::string &uniform, const glm::mat4 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix2v(const std::string &uniform, int count, glm::mat2 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix3v(const std::string &uniform, int count, glm::mat3 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix4v(const std::string &uniform, int count, glm::mat4 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix2x3(const std::string &uniform, const glm::mat2x3 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix3x2(const std::string &uniform, const glm::mat3x2 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix2x4(const std::string &uniform, const glm::mat2x4 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix4x2(const std::string &uniform, const glm::mat4x2 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x2fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix3x4(const std::string &uniform, const glm::mat3x4 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix4x3(const std::string &uniform, const glm::mat4x3 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x3fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformMatrix2x3v(const std::string &uniform, int count, glm::mat2x3 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x3fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix3x2v(const std::string &uniform, int count, glm::mat3x2 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x2fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix2x4v(const std::string &uniform, int count, glm::mat2x4 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix2x4fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix4x2v(const std::string &uniform, int count, glm::mat4x2 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x2fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix3x4v(const std::string &uniform, int count, glm::mat3x4 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix3x4fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniformMatrix4x3v(const std::string &uniform, int count, glm::mat4x3 *mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4x3fv(loc, count, GL_FALSE, &(*mat)[0][0]);
}

void Shader::SetUniform1ui(const std::string &uniform, unsigned int v)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1ui(loc, v);
}

void Shader::SetUniform2ui(const std::string &uniform, const glm::uvec2 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2ui(loc, vec.x, vec.y);
}

void Shader::SetUniform3ui(const std::string &uniform, const glm::uvec3 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3ui(loc, vec.x, vec.y, vec.z);
}

void Shader::SetUniform4ui(const std::string &uniform, const glm::uvec4 &vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform4ui(loc, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform1uiv(const std::string &uniform, int count, unsigned int *v)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform1uiv(loc, count, v);
}

void Shader::SetUniform2uiv(const std::string &uniform, int count, glm::uvec2 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform2uiv(loc, count, (unsigned int*)vec);
}

void Shader::SetUniform3uiv(const std::string &uniform, int count, glm::uvec3 *vec)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniform3uiv(loc, count, (unsigned int*)vec);
}

void Shader::SetUniform4uiv(const std::string &uniform, int count, glm::uvec4 *vec)
{
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
