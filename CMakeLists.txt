cmake_minimum_required(VERSION 3.0.0)

project(Isker VERSION 0.1.0)

set (source
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Game.cpp"
    "${PROJECT_SOURCE_DIR}/src/Game.hpp"
    "${PROJECT_SOURCE_DIR}/src/Input.cpp"
    "${PROJECT_SOURCE_DIR}/src/Input.hpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Renderer.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Renderer.hpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Shader.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Shader.hpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Texture.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Texture.hpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Font.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Font.hpp"
    "${PROJECT_SOURCE_DIR}/src/Component/Transform2D.cpp"
    "${PROJECT_SOURCE_DIR}/src/Component/Transform2D.hpp"
    "${PROJECT_SOURCE_DIR}/src/one_time_implements.c"
    "${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c"
    )

if (EMSCRIPTEN)
    add_executable(Isker ${source})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -s USE_SDL=2 -s USE_FREETYPE=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s EXIT_RUNTIME=1 -s ASYNCIFY --preload-file ${PROJECT_SOURCE_DIR}/asset@/asset")
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    add_custom_command(TARGET Isker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/platform/web/ $<TARGET_FILE_DIR:Isker>)
elseif (UNIX)
    add_executable(Isker ${source})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
    find_package(SDL2 REQUIRED)
    target_link_libraries(Isker dl m ${SDL2_LIBRARY})
elseif (WIN32)
    add_executable(Isker ${source})

    set(SDL_RENDER_ENABLED_BY_DEFAULT OFF CACHE BOOL "")
    set(SDL_TIMERS_ENABLED_BY_DEFAULT TRUE CACHE BOOL "")
    set(SDL_HAPTICS_ENABLED_BY_DEFAULT OFF CACHE BOOL "")
    set(SDL_FILESYSTEM_ENABLED_BY_DEFAULT OFF CACHE BOOL "")
    set(SDL_SENSOR_ENABLED_BY_DEFAULT OFF CACHE BOOL "")
    set(SDL_DLOPEN_ENABLED_BY_DEFAULT OFF CACHE BOOL "")

    set(VIDEO_DUMMY OFF CACHE BOOL "")
    set(DISKAUDIO OFF CACHE BOOL "")
    set(DUMMYAUDIO OFF CACHE BOOL "")
    set(HIDAPI OFF CACHE BOOL "")
    set(SSE OFF CACHE BOOL "")
    set(SSE2 OFF CACHE BOOL "")
    set(SSE3 OFF CACHE BOOL "")
    set(VIDEO_OPENGL TRUE CACHE BOOL "")
    set(VIDEO_OPENGLES TRUE CACHE BOOL "")
    set(DIRECTX OFF CACHE BOOL "")
    set(RENDER_D3D OFF CACHE BOOL "")
    set(RENDER_METAL OFF CACHE BOOL "")
    set(VIDEO_VIVANTE OFF CACHE BOOL "")
    set(VIDEO_METAL OFF CACHE BOOL "")
    set(VIDEO_VULKAN OFF CACHE BOOL "")

    set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "")

    add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/SDL")

    set(SDL2_INCLUDE_DIRS "")
    set(SDL2_LIBRARIES "SDL2-static" "SDL2main")

    if (MSVC)
        target_compile_features(Isker PRIVATE cxx_std_17)
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif ()

    target_link_libraries(Isker ${SDL2_LIBRARIES})
endif ()

if (NOT DEFINED EMSCRIPTEN)
    add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/freetype")
    target_link_libraries(Isker freetype)
    
    add_custom_command(TARGET Isker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/asset/ $<TARGET_FILE_DIR:Isker>/asset/)
endif ()

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "")
set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "")
add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/box2d")
target_link_libraries(Isker box2d)

target_include_directories(Isker PUBLIC
    "${SDL2_INCLUDE_DIRS}"
    "${PROJECT_SOURCE_DIR}/thirdparty/glad/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/glm"
    "${PROJECT_SOURCE_DIR}/thirdparty/stb"
    "${PROJECT_SOURCE_DIR}/thirdparty/box2d/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/entt/single_include"
    "${PROJECT_SOURCE_DIR}/thirdparty/freetype/include"
    )
