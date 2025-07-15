cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer CXX)

# --- CMake Settings ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- CPM.cmake Dependency Manager ---
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/deps/cpm_cache)
set(CPM_BINARY_DIR ${CMAKE_SOURCE_DIR}/deps/cpm_build)
include(cmake/CPM.cmake)

# --- System Dependencies ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)
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
set(PROJECTM_INSTALL_DIR ${CMAKE_SOURCE_DIR}/deps/projectm_install)
set(PROJECTM_LIB_PATH ${PROJECTM_INSTALL_DIR}/lib/libprojectM-4.so)

# Define the imported library target for projectM
file(MAKE_DIRECTORY "${PROJECTM_INSTALL_DIR}/include")
file(MAKE_DIRECTORY "${PROJECTM_INSTALL_DIR}/lib")
add_library(projectM_lib SHARED IMPORTED)
set_property(TARGET projectM_lib PROPERTY IMPORTED_LOCATION "${PROJECTM_LIB_PATH}")
set_property(TARGET projectM_lib PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROJECTM_INSTALL_DIR}/include")

# If the library doesn't exist, define the external project to build it
if(NOT EXISTS "${PROJECTM_LIB_PATH}")
    message(STATUS "projectM library not found. Building from source.")
    include(ExternalProject)
    ExternalProject_Add(
        projectM_external
        GIT_REPOSITORY https://github.com/projectM-visualizer/projectm.git
        GIT_TAG v4.1.0
        GIT_SHALLOW TRUE
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/deps/projectm_src"
        BINARY_DIR "${CMAKE_SOURCE_DIR}/deps/projectm_build"
        INSTALL_DIR "${PROJECTM_INSTALL_DIR}"
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBUILD_SHARED_LIBS=ON
        BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
        INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
    )
    # Make sure the library target depends on the external project build
    add_dependencies(projectM_lib projectM_external)
else()
    message(STATUS "Found pre-built projectM library at ${PROJECTM_LIB_PATH}")
endif()


# --- Source Files ---
set(APP_SOURCE_FILES
    src/main.cpp
    src/core.cpp
    src/ConfigLoader.cpp
    src/CliParser.cpp
    src/renderer.cpp
    src/event_handler.cpp
    src/audio_input.cpp
    src/preset_manager.cpp
    src/AnimationManager.cpp
    src/TextRenderer.cpp
    src/TextManager.cpp
    src/QtGui.cpp
    src/VideoExporter.cpp
    src/backends/audio_backend.cpp
    src/backends/display_backend.cpp
    src/utils/common.cpp
    src/utils/Logger.cpp
)

# --- Qt MOC Generation ---
qt6_wrap_cpp(MOC_SOURCES include/QtGui.h)

# --- Executable Target ---
add_executable(AuroraVisualizer ${APP_SOURCE_FILES} ${MOC_SOURCES})

# --- Target Properties (Includes and Linking) ---
target_include_directories(AuroraVisualizer PRIVATE
    "include"
    "${PROJECTM_INSTALL_DIR}/include"
    "${FREETYPE_INCLUDE_DIRS}"
    "${tomlplusplus_SOURCE_DIR}/include"
)

target_link_libraries(AuroraVisualizer PRIVATE
    # Project Libraries
    projectM_lib
    tomlplusplus::tomlplusplus

    # System Libraries
    Qt6::Widgets
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    ${FREETYPE_LIBRARIES}
    OpenGL::GL
    GLEW::GLEW
    glm::glm
    PkgConfig::FFMPEG
)

# Get current timestamp
string(TIMESTAMP CMAKE_BUILD_TIMESTAMP "%Y%m%d_%H%M%S")

# Custom command to copy the executable to backups with a timestamp
add_custom_command(TARGET AuroraVisualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:AuroraVisualizer>
            ${CMAKE_SOURCE_DIR}/backups/AuroraVisualizer_${CMAKE_BUILD_TIMESTAMP}
    COMMENT "Backing up AuroraVisualizer executable to backups directory."
)