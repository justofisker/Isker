#include "Texture.hpp"

#include <stb/stb_image.h>
#include <glad/glad.h>

Texture::Texture(const std::string &file_path)
{
    stbi_set_flip_vertically_on_load(1);
    int w, h, c;
    unsigned char *data = stbi_load(file_path.c_str(), &w, &h, &c, 4);
    if(!data)
    {
        fprintf(stderr, "Cannot load image file %s\nSTB Reason: %s\n", file_path.c_str(), stbi_failure_reason());
        return;
    }

    width = w;
    height = h;
    channels = c;

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

}


void Texture::Bind(unsigned char slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}