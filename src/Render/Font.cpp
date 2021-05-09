#include "Font.hpp"

#include <freetype/freetype.h>
#include <SDL_log.h>
#include <glad/glad.h>

Font::Font(const FontBuilder &fontBuilder, const std::string& font_path, int font_size)
    : m_Characters(std::unique_ptr<FontCharacter[]>(new FontCharacter[128])), m_FontSize(font_size)
{
    const unsigned int texture_rows = 11;
    const unsigned int texture_size = texture_rows * font_size;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        texture_size,
        texture_size,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_Texture = std::make_shared<FontTexture>(texture, texture_size);

    FT_Face face;
    if(FT_New_Face((FT_Library)fontBuilder.s_FreetypeLibrary, font_path.c_str(), 0, &face) )
    {
        SDL_Log("FAILED TO CREATE FONT %s.\n", font_path.c_str());
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            SDL_Log("Typetype failed to load char\n");
            continue;
        }

        struct Pixel {
            unsigned char r, g, b, a;
        };

        auto pixels = std::unique_ptr<Pixel[]>(new Pixel[face->glyph->bitmap.width * face->glyph->bitmap.rows]);

        for(int i = 0; i < face->glyph->bitmap.width * face->glyph->bitmap.rows; i++)
        {
            Pixel &pixel = pixels[i];
            if(face->glyph->bitmap.buffer[i])
            {
                pixel.r = pixel.g = pixel.b = 255;
                pixel.a = face->glyph->bitmap.buffer[i];
            } else
            {
                pixel.r = pixel.g = pixel.b = pixel.a = 0;
            }
        }

        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            (c % texture_rows) * font_size,
            (c / texture_rows) * font_size,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels.get()
        );

        m_Characters[c] = FontCharacter {
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x,
            glm::vec2((float)((c % texture_rows) * font_size) / texture_size, (float)((c / texture_rows) * font_size) / texture_size),
            glm::vec2((float)((c % texture_rows) * font_size + face->glyph->bitmap.width) / texture_size, (float)((c / texture_rows) * font_size + face->glyph->bitmap.rows) / texture_size)
        };
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

const Font::FontCharacter &Font::GetCharacter(unsigned char c) const
{
    return m_Characters[c];
}

Font::~Font()
{

}

Font::FontTexture::FontTexture(unsigned int textureID, unsigned int size)
    : Texture(textureID, glm::ivec2(size), 4)
{

}

unsigned int FontBuilder::s_FontCreatorCount = 0;
void *FontBuilder::s_FreetypeLibrary = nullptr;

FontBuilder::FontBuilder()
{
    if(!s_FontCreatorCount) CreateFontContext();
    s_FontCreatorCount++;
}

FontBuilder::~FontBuilder()
{
    s_FontCreatorCount--;
    if(!s_FontCreatorCount) DestroyFontContect();
}

std::shared_ptr<Font> FontBuilder::CreateFont(const std::string &font_path, int font_size)
{
    return std::make_shared<Font>(*this, font_path, font_size);
}

void FontBuilder::CreateFontContext()
{
    if (FT_Init_FreeType((FT_Library*)&s_FreetypeLibrary))
    {
        SDL_Log("Cannot Init FreeType!\n");
        return;
    }
}

void FontBuilder::DestroyFontContect()
{
    FT_Done_FreeType((FT_Library)s_FreetypeLibrary);
    s_FreetypeLibrary = nullptr;
}
