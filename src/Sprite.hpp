#pragma once

#include <string>

class Sprite {
private:
    unsigned int m_TextureID;
    int width, height, channels;
public:
    Sprite() = delete;
    Sprite(const std::string &file_path);
    void Bind(unsigned char slot) const;

    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
    inline int GetChannels() const { return channels; }
};