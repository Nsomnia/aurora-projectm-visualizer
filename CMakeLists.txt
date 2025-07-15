cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer CXX)

# --- CPM.cmake Dependency Manager ---
include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- System Dependencies ---
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavcodec libavformat libavutil libswscale)

# --- CPM Dependencies ---
CPMAddPackage(
    NAME tomlplusplus
    GITHUB_REPOSITORY marzer/tomlplusplus
    VERSION 3.4.0 # Using a specific version for stability
)

# --- External Project: projectM ---
include(ExternalProject)
ExternalProject_Add(
    projectM_external
    GIT_REPOSITORY https://github.com/projectM-visualizer/projectm.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE
    SOURCE_DIR "${CMAKE_BINARY_DIR}/projectm_src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/projectm_build"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/projectm_install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_SHARED_LIBS=ON
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)
# Create the install directory for projectM ahead of time to prevent CMake configuration errors
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/projectm_install/include")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/projectm_install/lib")
add_library(projectM_lib SHARED IMPORTED)
set_property(TARGET projectM_lib PROPERTY IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/projectm_install/lib/libprojectM-4.so")
set_property(TARGET projectM_lib PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/projectm_install/include")
add_dependencies(projectM_lib projectM_external)

# --- Source Files ---
file(GLOB_RECURSE APP_SOURCE_FILES "src/*.cpp" "src/**/*.cpp")
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

# --- Target Properties (Includes and Linking) ---
target_include_directories(AuroraVisualizer PRIVATE
    "include"
    "deps/imgui"
    "${CMAKE_BINARY_DIR}/projectm_install/include"
    "${FREETYPE_INCLUDE_DIRS}"
    "${CMAKE_BINARY_DIR}/_deps/tomlplusplus-src"
)

target_link_libraries(AuroraVisualizer PRIVATE
    # Project Libraries
    projectM_lib
    tomlplusplus::tomlplusplus

    # System Libraries
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    ${FREETYPE_LIBRARIES}
    OpenGL::GL
    GLEW::GLEW
    glm::glm
    PkgConfig::FFMPEG
)
