#pragma once

#ifdef __EMSCRIPTEN__
    #define ISKERAPI
#else
    #ifdef DLL_BUILD
        #define ISKERAPI __declspec(dllexport)
    #else
        #define ISKERAPI __declspec(dllimport)
    #endif // DLL_BUILD
#endif // __EMSCRIPTEN__