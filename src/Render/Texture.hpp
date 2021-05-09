#pragma once

#include <string>
#include <memory>

#include <SDL_stdinc.h>
#include <glm/vec2.hpp>

class Texture {
private:
    unsigned int m_TextureID;
    int m_Channels;
    glm::ivec2 m_Size;
protected:
    Texture(unsigned int textureID, const glm::ivec2 &size, int channels);
    void InvalidateTextureID() { m_TextureID = SDL_MAX_UINT32; }
public:
    struct TextureUV {
        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };
public:
    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture(const std::string &file_path);
    virtual ~Texture();
    void Bind(unsigned char slot) const;

    virtual const TextureUV &GetUV() const { static TextureUV uv{glm::vec2(0.0f), glm::vec2(1.0f)}; return uv; };

    inline unsigned int GetTextureID() const { return m_TextureID; }
    inline int GetWidth() const { return m_Size.x; }
    inline int GetHeight() const { return m_Size.y; }
    inline const glm::ivec2 &GetSize() const { return m_Size; }
    inline int GetChannels() const { return m_Channels; }
};

class SubTexture : public Texture {
private:
    TextureUV m_UV;
    std::shared_ptr<Texture> m_ParentTexture;
public:
    SubTexture() = delete;
    SubTexture(const SubTexture&) = delete;
    SubTexture(std::shared_ptr<Texture> texture, int top, int left, int bottom, int right);
    ~SubTexture();
    virtual const TextureUV &GetUV() const { return m_UV; };
};