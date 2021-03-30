#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifndef EMSCRIPTEN
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif