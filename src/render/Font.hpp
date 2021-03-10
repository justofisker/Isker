#pragma once

#include <string>
#include <memory>
#include <array>

#include <glm/vec2.hpp>

#include "Texture.hpp"

class Font;

class FontBuilder
{
private:
    static unsigned int s_FontCreatorCount;
    static void* s_FreetypeLibrary;
public:
    FontBuilder();
    FontBuilder(const FontBuilder&) = delete;
    ~FontBuilder();
    std::shared_ptr<Font> CreateFont(const std::string &font_path, int font_size);
private:
    void CreateFontContext();
    void DestroyFontContect();
    friend class Font;
};

class Font {
public:
    struct FontCharacter {
        glm::ivec2 size;
        glm::ivec2 bearing;
        signed long advance;
        glm::vec2 bottomLeftUV;
        glm::vec2 topRightUV;
    };
public:
    Font(const FontBuilder &fontBuilder, const std::string &font_path, int font_size);
    Font(const Font&) = delete;
    inline std::shared_ptr<Texture> GetTexture() const { return m_Texture; };
    const FontCharacter &GetCharacter(unsigned char c) const;
    inline unsigned int GetFontSize() const { return m_FontSize; }
    ~Font();
private:
    class FontTexture : public Texture
    {
    public:
        FontTexture(unsigned int textureID, unsigned int size);
    };
    std::unique_ptr<FontCharacter[]> m_Characters;
    std::shared_ptr<FontTexture> m_Texture;
    unsigned int m_FontSize;
};