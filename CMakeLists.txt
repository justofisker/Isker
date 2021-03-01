cmake_minimum_required(VERSION 3.0.0)

set(game_name Isker)

project(${game_name} VERSION 0.1.0)

set (source
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/Game.cpp"
    "${PROJECT_SOURCE_DIR}/src/Renderer.cpp"
    "${PROJECT_SOURCE_DIR}/src/Input.cpp"
    "${PROJECT_SOURCE_DIR}/src/Texture.cpp"
    "${PROJECT_SOURCE_DIR}/src/Mesh.cpp"
    "${PROJECT_SOURCE_DIR}/src/Shader.cpp"
    "${PROJECT_SOURCE_DIR}/thirdparty/glad/src/glad.c"
    "${PROJECT_SOURCE_DIR}/src/one_time_implements.c"
    )

if (UNIX)
    add_executable(${game_name} ${source})
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
    find_package(SDL2 REQUIRED)
    target_link_libraries(${game_name} dl m ${SDL2_LIBRARY})
elseif (WIN32)
    add_executable(${game_name} ${source})
    if (MSVC)
        target_link_libraries(${game_name} "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/x86_64-VS/SDL2.lib")
        add_compile_definitions(SDL_MAIN_HANDLED)
    else (MSVC)
        target_link_libraries(${game_name} "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/x86_64-w64-mingw32/lib/libSDL2.dll.a")
        add_compile_definitions(SDL_MAIN_HANDLED)
    endif (MSVC)
    add_custom_command(TARGET ${game_name} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_SOURCE_DIR}/thirdparty/SDL2/x86_64-w64-mingw32/bin/SDL2.dll $<TARGET_FILE_DIR:Isker>/SDL2.dll)
endif (UNIX)

target_compile_features(${game_name} PRIVATE cxx_std_17)

add_custom_command(TARGET ${game_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/asset/ $<TARGET_FILE_DIR:Isker>/asset/)

add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/freetype")
target_link_libraries(${game_name} freetype)

add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/box2d")
target_link_libraries(${game_name} box2d)
        

target_include_directories(${game_name} PUBLIC
    "${PROJECT_SOURCE_DIR}/thirdparty/SDL2/x86_64-w64-mingw32/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/glad/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/glm"
    "${PROJECT_SOURCE_DIR}/thirdparty/stb"
    "${PROJECT_SOURCE_DIR}/thirdparty/box2d/include"
    )