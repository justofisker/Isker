#pragma once

#include <string>

class Sprite {
public:
    Sprite() = delete;
    Sprite(const std::string &file_path);
};