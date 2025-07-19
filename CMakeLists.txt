cmake_minimum_required(VERSION 3.20)
project(AuroraVisualizer VERSION 0.2.0 LANGUAGES CXX)

# --- Pre-build Reminder ---
message(STATUS "Reminder: Consider committing your changes before starting a build. Use 'git status' to see your changes.")

# --- CMake Settings ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Versioning ---
# Configure a header file to pass the version number to the source code
configure_file(
    "${CMAKE_SOURCE_DIR}/include/version.h.in"
    "${CMAKE_BINARY_DIR}/include/version.h"
)

# Configure a header file for other paths and definitions
set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")
configure_file(
    "${CMAKE_SOURCE_DIR}/include/config.h.in"
    "${CMAKE_BINARY_DIR}/include/config.h"
)

# --- CPM.cmake Dependency Manager ---
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/deps/cpm_cache)
include(cmake/CPM.cmake)

# --- System Dependencies ---
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavcodec libavformat libavutil libswscale)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

# --- CPM Dependencies ---
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

if(NOT EXISTS "${PROJECTM_LIB_PATH}")
    message(STATUS "projectM library not found. Please ensure it is built and installed in ${PROJECTM_INSTALL_DIR}")
    # The ExternalProject_Add block has been removed to favor a manual build process.
    # To build projectM, navigate to deps/projectm_src and follow their build instructions.
    # Example:
    # cd deps/projectm_src
    # mkdir build && cd build
    # cmake .. -DCMAKE_INSTALL_PREFIX=../../projectm_install -DBUILD_SHARED_LIBS=ON -DPROJECTM_ENABLE_QT5=OFF
    # cmake --build . --target install
else()
    message(STATUS "Found pre-built projectM library at ${PROJECTM_LIB_PATH}")
endif()

# --- Source Files ---
file(GLOB_RECURSE APP_SOURCE_FILES 
    "src/*.cpp" 
    "src/utils/*.cpp"
    "src/*.vert" 
    "src/*.frag"
)

# --- Qt MOC Generation ---
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
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

# --- Post-Build Message ---
add_custom_command(TARGET AuroraVisualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build successful! Run with ./build/bin/AuroraVisualizer. Consider committing your changes."
    COMMENT " "
)

# --- Custom Backup Target ---
string(TIMESTAMP CURRENT_TIME "%Y%m%d_%H%M%S")
set(BACKUP_FILE_NAME "${CMAKE_SOURCE_DIR}/backups/aurora-visualizer-v${PROJECT_VERSION}-${CURRENT_TIME}.tar.gz")
add_custom_target(create_backup
    COMMAND ${CMAKE_COMMAND} -E tar "czf" ${BACKUP_FILE_NAME}
        --files-from=- <<EOT
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/docs
        ${CMAKE_SOURCE_DIR}/config
        ${CMAKE_SOURCE_DIR}/CMakeLists.txt
        ${CMAKE_SOURCE_DIR}/README.md
        $<TARGET_FILE:AuroraVisualizer>
EOT
    DEPENDS AuroraVisualizer
    COMMENT "Creating a full project backup in ${BACKUP_FILE_NAME}"
)

# --- Install Rules ---
install(TARGETS AuroraVisualizer RUNTIME DESTINATION bin)
install(FILES config/default.toml DESTINATION etc)

message(STATUS "Configuration complete. Run 'cmake --build build' to compile.")
