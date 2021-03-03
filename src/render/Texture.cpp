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

    m_Size = glm::vec2(w, h);
    m_Channels = c;

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

Texture::~Texture()
{
    if(m_TextureID != SDL_MAX_UINT32) glDeleteTextures(1, &m_TextureID);
}

Texture::Texture(unsigned int textureID, const glm::vec2 &size, int channels)
    : m_TextureID(textureID), m_Size(size), m_Channels(channels)
{

}

void Texture::Bind(unsigned char slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

SubTexture::SubTexture(std::shared_ptr<Texture> texture, int top, int left, int bottom, int right)
    : Texture(texture->GetTextureID(), glm::ivec2(right - left, bottom - top), texture->GetChannels()), m_ParentTexture(texture)
{
    TextureUV uv = texture->GetUV();
    glm::ivec2 size = texture->GetSize();
    m_UV.bottomLeft = glm::vec2(
                    uv.bottomLeft.x + ((float)left / size.x) * (uv.topRight.x - uv.bottomLeft.x),
                    uv.bottomLeft.y + ((float)(size.y - bottom) / size.y) * (uv.topRight.y - uv.bottomLeft.y)
                );
    m_UV.topRight = glm::vec2(
                    uv.topRight.x - ((float)(size.x - right) / size.x) * (uv.topRight.x - uv.bottomLeft.x),
                    uv.topRight.y - ((float)top / size.y) * (uv.topRight.y - uv.bottomLeft.y)
                );
}

SubTexture::~SubTexture()
{
    InvalidateTextureID();
}
