cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer VERSION 0.2.0 LANGUAGES CXX)

# --- CMake Settings ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# --- System Dependencies ---
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavcodec libavformat libavutil libswscale)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# --- CPM.cmake Dependency Manager ---
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/deps/cpm_cache)
include(cmake/CPM.cmake)

CPMAddPackage(
    NAME tomlplusplus
    GITHUB_REPOSITORY marzer/tomlplusplus
    VERSION 3.4.0
)

# --- External Project: projectM ---
set(PROJECTM_INSTALL_DIR ${CMAKE_SOURCE_DIR}/deps/projectm_install)
set(PROJECTM_LIB_PATH ${PROJECTM_INSTALL_DIR}/lib/libprojectM-4.so) # Correct path for v4
add_library(projectM_lib SHARED IMPORTED GLOBAL)
set_property(TARGET projectM_lib PROPERTY IMPORTED_LOCATION "${PROJECTM_LIB_PATH}")
set_property(TARGET projectM_lib PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECTM_INSTALL_DIR}/include")

# --- Source Files ---
set(APP_SOURCE_FILES
    src/AnimationManager.cpp
    src/audio_input.cpp
    src/CliParser.cpp
    src/ConfigLoader.cpp
    src/core.cpp
    src/event_handler.cpp
    src/main.cpp
    src/preset_manager.cpp
    src/QtGui.cpp
    src/QtOpenGLWidget.cpp
    src/renderer.cpp
    src/TextManager.cpp
    src/TextRenderer.cpp
    src/VideoExporter.cpp
    src/backends/audio_backend.cpp
    src/backends/display_backend.cpp
    src/utils/common.cpp
    src/utils/gl_utils.cpp
    src/utils/Logger.cpp
    src/fbo.vert
    src/fbo.frag
)

# --- Qt MOC Generation ---
qt6_wrap_cpp(MOC_SOURCES include/QtGui.h include/QtOpenGLWidget.h)

# --- Executable Target ---
add_executable(AuroraVisualizer ${APP_SOURCE_FILES} ${MOC_SOURCES})

# --- Target Properties (Includes and Linking) ---
target_include_directories(AuroraVisualizer PRIVATE
    "include"
    "${CMAKE_BINARY_DIR}/include" # For version.h
    "${PROJECTM_INSTALL_DIR}/include"
    "${FREETYPE_INCLUDE_DIRS}"
    "${tomlplusplus_SOURCE_DIR}/include"
)

target_link_libraries(AuroraVisualizer PRIVATE
    projectM_lib
    tomlplusplus::tomlplusplus
    Qt6::Widgets Qt6::OpenGLWidgets
    ${FREETYPE_LIBRARIES}
    OpenGL::GL glm::glm
    PkgConfig::FFMPEG
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
)