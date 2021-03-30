cmake_minimum_required(VERSION 3.0.0)

project(Isker VERSION 0.1.0)

set (source
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Game.cpp"
    "${PROJECT_SOURCE_DIR}/src/Input.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Renderer.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Shader.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Texture.cpp"
    "${PROJECT_SOURCE_DIR}/src/Render/Font.cpp"
    "${PROJECT_SOURCE_DIR}/src/Component/Transform2D.cpp"
    "${PROJECT_SOURCE_DIR}/src/one_time_implements.c"
    "${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c"
    )

if (EMSCRIPTEN)
    add_executable(Isker ${source})
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "-Wc++17-extensions")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -s USE_SDL=2 -s USE_FREETYPE=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s EXIT_RUNTIME=1 -s ASYNCIFY --preload-file ${PROJECT_SOURCE_DIR}/asset@/asset")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif (UNIX)
    add_executable(Isker ${source})
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
    find_package(SDL2 REQUIRED)
    target_link_libraries(Isker dl m ${SDL2_LIBRARY})
elseif (WIN32)
    add_executable(Isker ${source})
    if (MSVC)
        target_link_libraries(Isker "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/x86_64-VS/SDL2.lib")
        add_compile_definitions(SDL_MAIN_HANDLED)
    else (MSVC)
        target_link_libraries(Isker "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/x86_64-w64-mingw32/lib/libSDL2.dll.a")
        add_compile_definitions(SDL_MAIN_HANDLED)
    endif (MSVC)
    add_custom_command(TARGET Isker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/thirdparty/SDL2/x86_64-w64-mingw32/bin/SDL2.dll $<TARGET_FILE_DIR:Isker>/SDL2.dll)
    add_custom_command(TARGET Isker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/asset/ $<TARGET_FILE_DIR:Isker>/asset/)
endif (EMSCRIPTEN)

if (NOT DEFINED EMSCRIPTEN)
    add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/freetype")
    target_link_libraries(Isker freetype)

    # add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/box2d")
    # target_link_libraries(Isker box2d)
endif ()
        
target_include_directories(Isker PUBLIC
    "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/x86_64-w64-mingw32/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/glad/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/glm"
    "${PROJECT_SOURCE_DIR}/thirdparty/stb"
    "${PROJECT_SOURCE_DIR}/thirdparty/box2d/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/entt/single_include"
    "${PROJECT_SOURCE_DIR}/thirdparty/freetype/include"
    )
