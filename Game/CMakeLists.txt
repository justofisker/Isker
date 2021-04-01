cmake_minimum_required(VERSION 3.0.0)

project(Game VERSION 0.1.0)

add_executable(Game
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    )

target_include_directories(Game PUBLIC
    "${PROJECT_SOURCE_DIR}/../Isker/src"
    )

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -s USE_SDL=2 -s USE_FREETYPE=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s EXIT_RUNTIME=1 -s ASYNCIFY --preload-file ${PROJECT_SOURCE_DIR}/asset@/asset")
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/../Isker/platform/web/ $<TARGET_FILE_DIR:Game>)
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
    find_package(SDL2 REQUIRED)
    target_link_libraries(Isker dl m ${SDL2_LIBRARY})
elseif (WIN32)
    if (MSVC)
        target_compile_features(Isker PRIVATE cxx_std_17)
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif ()
endif ()

if (NOT DEFINED EMSCRIPTEN)
    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/asset/ $<TARGET_FILE_DIR:Game>/asset/)
endif ()

target_link_libraries(Game Isker)