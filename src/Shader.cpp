#include "Shader.hpp"
#include <glad/glad.h>
#include <SDL2/SDL_log.h>

#include <fstream>
#include <vector>

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

void Shader::SetUniformMat4(const std::string &uniform, const glm::mat4 &mat)
{
    int loc = GetUniformLoc(uniform);
    if(loc == -1) return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
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
