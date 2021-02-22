#pragma once

#include "Singleton.hpp"

class Game {
    SINGLETON(Game);
public:
    void Init();
    void Frame();
};