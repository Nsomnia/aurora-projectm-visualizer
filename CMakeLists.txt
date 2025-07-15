cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
# Find system libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

# Find FFmpeg libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavcodec libavformat libavutil libswscale)


# --- External Project: projectM ---
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
        -DBUILD_SHARED_LIBS=ON
        -DPROJECTM_BUILD_TESTS=OFF
        -DPROJECTM_BUILD_SDL_UI=OFF
        -DPROJECTM_BUILD_GLM_EXTERNAL=OFF
        -DPROJECTM_BUILD_PROJECTM_EVAL_EXTERNAL=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/projectm_build
    INSTALL_COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR}/projectm_build
    LOG_CONFIGURE TRUE
    LOG_BUILD TRUE
    LOG_INSTALL TRUE
)

# Create the install directory for projectM ahead of time to prevent CMake configuration errors
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/projectm_install/include")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/projectm_install/lib")

# Create an imported library target for the library built by ExternalProject
add_library(projectM_lib SHARED IMPORTED)
set_property(TARGET projectM_lib PROPERTY
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/projectm_install/lib/libprojectM-4.so"
)
set_property(TARGET projectM_lib PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/projectm_install/include"
)
add_dependencies(projectM_lib projectM_external)


# --- Source Files ---
# Using GLOB is convenient for rapid development but is not ideal for stable projects.
# It's recommended to list files explicitly in the long run.
file(GLOB_RECURSE APP_SOURCE_FILES
    "src/*.cpp"
    "src/**/*.cpp"
)

set(IMGUI_SOURCE_FILES
    "deps/imgui/imgui.cpp"
    "deps/imgui/imgui_draw.cpp"
    "deps/imgui/imgui_widgets.cpp"
    "deps/imgui/imgui_tables.cpp"
    "deps/imgui/backends/imgui_impl_sdl2.cpp"
    "deps/imgui/backends/imgui_impl_opengl3.cpp"
)

# --- Executable Target ---
add_executable(AuroraVisualizer ${APP_SOURCE_FILES} ${IMGUI_SOURCE_FILES})

# --- Linking and Include Directories ---
target_include_directories(AuroraVisualizer PRIVATE
    "include"
    "deps/imgui"
    "${CMAKE_BINARY_DIR}/projectm_install/include"
    "/usr/include/freetype2"
)

target_link_libraries(AuroraVisualizer PRIVATE
    # Our imported projectM library
    projectM_lib

    # System libraries found via find_package
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    ${FREETYPE_LIBRARIES}
    OpenGL::GL
    GLEW::GLEW
    glm::glm

    # FFmpeg libraries
    PkgConfig::FFMPEG
)
