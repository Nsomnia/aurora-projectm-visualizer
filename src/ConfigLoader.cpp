// src/ConfigLoader.cpp
#include "ConfigLoader.h"
#include "Config.h"
#include "utils/Logger.h"
#include <toml++/toml.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Helper function to resolve paths, expanding ~
static std::string resolve_path(const std::string& path_str) {
    if (path_str.empty()) {
        return "";
    }
    if (path_str[0] == '~') {
        const char* home = getenv("HOME");
        if (home) {
            return std::string(home) + path_str.substr(1);
        }
    }
    return path_str;
}

bool ConfigLoader::load(Config& config, const std::string &executable_path_str) {
    fs::path config_path_to_load;

    // 1. Check for user-specific config in ~/.config/aurora-visualizer/config.toml
    const char* home_dir = getenv("HOME");
    if (home_dir) {
        fs::path user_config_path = fs::path(home_dir) / ".config" / "aurora-visualizer" / "config.toml";
        if (fs::exists(user_config_path)) {
            config_path_to_load = user_config_path;
            Logger::info("Loading user config from: " + user_config_path.string());
        }
    }

    // 2. If no user config, fall back to default.toml in the project's config directory
    if (config_path_to_load.empty()) {
        fs::path exe_dir = fs::path(executable_path_str).parent_path();
        // This assumes the executable is in 'build/', so we go up one level.
        // This might need to be more robust for system-wide installs.
        fs::path project_root = exe_dir.parent_path();
        fs::path default_config_path = project_root / "config" / "default.toml";

        if (fs::exists(default_config_path)) {
            config_path_to_load = default_config_path;
            Logger::info("Loading default config from: " + default_config_path.string());
        }
    }

    if (config_path_to_load.empty() || !fs::exists(config_path_to_load)) {
        Logger::warn("No configuration file found. Using built-in default values.");
        return true; // Not a fatal error
    }

    try {
        toml::table tbl = toml::parse_file(config_path_to_load.string());

        // --- Display & Performance ---
        config.width = tbl["display"]["width"].value_or(config.width);
        config.height = tbl["display"]["height"].value_or(config.height);
        config.fps = tbl["display"]["fps"].value_or(config.fps);

        // --- Text & Font ---
        config.font_path = resolve_path(tbl["text"]["font_path"].value_or(config.font_path));
        config.songInfoFontSize = tbl["text"]["song_info_font_size"].value_or(config.songInfoFontSize);
        config.urlFontSize = tbl["text"]["url_font_size"].value_or(config.urlFontSize);
        config.songInfoFontColor = hexToVec3(tbl["text"]["song_info_color"].value_or("FFFFFF"));
        config.urlFontColor = hexToVec3(tbl["text"]["url_color"].value_or("00A3B1"));
        config.songInfoBorderColor = hexToVec3(tbl["text"]["song_info_border_color"].value_or("000000"));
        config.urlBorderColor = hexToVec3(tbl["text"]["url_border_color"].value_or("000000"));
        config.songInfoBorderThickness = tbl["text"]["song_info_border_thickness"].value_or(config.songInfoBorderThickness);
        config.urlBorderThickness = tbl["text"]["url_border_thickness"].value_or(config.urlBorderThickness);

        // --- Text Content & Visibility ---
        config.artistName = tbl["text"]["artist_name"].value_or(config.artistName);
        config.urlText = tbl["text"]["url_text"].value_or(config.urlText);
        config.show_song_title = tbl["text"]["show_song_title"].value_or(config.show_song_title);
        config.show_artist_name = tbl["text"]["show_artist_name"].value_or(config.show_artist_name);
        config.show_url = tbl["text"]["show_url"].value_or(config.show_url);
        config.show_text_border = tbl["text"]["show_text_border"].value_or(config.show_text_border);

        // --- Recording ---
        config.enable_recording = tbl["recording"]["enable_recording"].value_or(config.enable_recording);
        config.video_directory = resolve_path(tbl["recording"]["video_directory"].value_or(config.video_directory));
        config.video_framerate = tbl["recording"]["video_framerate"].value_or(config.video_framerate);
        config.ffmpeg_command = tbl["recording"]["ffmpeg_command"].value_or(config.ffmpeg_command);

        // ... (add other sections like animation, presets, etc. in the same way)

    } catch (const toml::parse_error& err) {
        Logger::error("Failed to parse config file: " + std::string(err.what()));
        return false;
    }

    return true;
}


