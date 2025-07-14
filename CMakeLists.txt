cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui
)

# Source files
file(GLOB_RECURSE APP_SOURCE_FILES
    "src/*.cpp"
    "src/**/*.cpp"
    "src/PipewireAudioRouter.cpp"
)

set(IMGUI_SOURCE_FILES
    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui_draw.cpp"
    "deps/imgui/imgui_widgets.cpp"
    "deps/imgui/imgui_tables.cpp"
    "deps/imgui/backends/imgui_impl_sdl2.cpp"
    "deps/imgui/backends/imgui_impl_opengl3.cpp"
)

set(SOURCE_FILES ${APP_SOURCE_FILES} ${IMGUI_SOURCE_FILES})

include(ExternalProject)

ExternalProject_Add(
    projectM_external
    GIT_REPOSITORY https://github.com/projectM-visualizer/projectm.git
    GIT_TAG v4.1.0 # Use a specific tag for reproducibility
    GIT_SHALLOW TRUE
    SOURCE_DIR "${CMAKE_BINARY_DIR}/projectm_src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/projectm_build"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/projectm_install"
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_SHARED_LIBS=ON # Build as shared library
        -DPROJECTM_BUILD_TESTS=OFF
        -DPROJECTM_BUILD_SDL_UI=OFF
        -DPROJECTM_BUILD_GLM_EXTERNAL=OFF # Use projectM's internal GLM or system GLM if available
        -DPROJECTM_BUILD_PROJECTM_EVAL_EXTERNAL=OFF # Use projectM's internal projectM-eval
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)

# Add executable
find_package(SDL2 REQUIRED)

add_executable(AuroraVisualizer ${SOURCE_FILES})

# Add projectM_external as a dependency for AuroraVisualizer
add_dependencies(AuroraVisualizer projectM_external)

# Link necessary libraries (placeholders for now, will be refined)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

find_package(SDL2_mixer REQUIRED)

target_link_libraries(AuroraVisualizer
    PRIVATE
    SDL2::SDL2
    OpenGL::GL
    GLEW::GLEW
    Freetype::Freetype
    SDL2_mixer::SDL2_mixer
    # Link against the installed projectM library
    ${CMAKE_BINARY_DIR}/projectm_install/lib/libprojectM-4.so
    # Placeholder for actual libraries like FFmpeg
    # These will be added as we integrate them properly with CMake
)

target_include_directories(AuroraVisualizer
    PUBLIC
        ${CMAKE_BINARY_DIR}/projectm_install/include
)
