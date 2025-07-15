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

// Helper to merge two TOML tables. `base` is updated with values from `overlay`.
void merge_toml_tables(toml::table& base, const toml::table& overlay) {
    overlay.for_each([&](const auto& key, const auto& value) {
        if (value.is_table()) {
            if (!base.contains(key) || !base[key].is_table()) {
                base.insert(key, toml::table{});
            }
            merge_toml_tables(*base[key].as_table(), *value.as_table());
        } else {
            base.insert_or_assign(key, value);
        }
    });
}


bool ConfigLoader::load(Config& config, const std::string &executable_path_str) {
    toml::table merged_config;

    // 1. Load default config
    fs::path exe_dir = fs::path(executable_path_str).parent_path();
    fs::path project_root = exe_dir.parent_path();
    fs::path default_config_path = project_root / "config" / "default.toml";

    if (fs::exists(default_config_path)) {
        try {
            merged_config = toml::parse_file(default_config_path.string());
            Logger::info("Loading default config from: " + default_config_path.string());
        } catch (const toml::parse_error& err) {
            Logger::error("Failed to parse default config file: " + std::string(err.what()));
            return false; // Default config must be valid
        }
    } else {
        Logger::warn("Default configuration file not found. Using built-in values.");
    }

    // 2. Load user config and merge over default
    const char* home_dir = getenv("HOME");
    if (home_dir) {
        fs::path user_config_dir = fs::path(home_dir) / ".config" / "aurora-visualizer";
        fs::create_directories(user_config_dir); // Ensure the directory exists
        fs::path user_config_path = user_config_dir / "config.toml";
        if (fs::exists(user_config_path)) {
            try {
                toml::table user_config = toml::parse_file(user_config_path.string());
                Logger::info("Loading user config from: " + user_config_path.string());
                merge_toml_tables(merged_config, user_config);
            } catch (const toml::parse_error& err) {
                Logger::error("Failed to parse user config file: " + std::string(err.what()));
                // Continue with default config if user's is invalid
            }
        }
    }

    // 3. Populate the Config struct from the merged table
    if (merged_config.contains("display")) {
        const auto& display = *merged_config["display"].as_table();
        config.width = display["width"].value_or(config.width);
        config.height = display["height"].value_or(config.height);
        config.fps = display["fps"].value_or(config.fps);
    }

    if (merged_config.contains("text")) {
        const auto& text = *merged_config["text"].as_table();
        config.font_path = resolve_path(text["font_path"].value_or(config.font_path));
        config.songInfoFontSize = text["song_info_font_size"].value_or(config.songInfoFontSize);
        config.urlFontSize = text["url_font_size"].value_or(config.urlFontSize);
        config.songInfoFontColor = hexToVec3(text["song_info_color"].value_or("FFFFFF"));
        config.urlFontColor = hexToVec3(text["url_color"].value_or("00A3B1"));
        config.songInfoBorderColor = hexToVec3(text["song_info_border_color"].value_or("000000"));
        config.urlBorderColor = hexToVec3(text["url_border_color"].value_or("000000"));
        config.songInfoBorderThickness = text["song_info_border_thickness"].value_or(config.songInfoBorderThickness);
        config.urlBorderThickness = text["url_border_thickness"].value_or(config.urlBorderThickness);
        config.artistName = text["artist_name"].value_or(config.artistName);
        config.urlText = text["url_text"].value_or(config.urlText);
        config.show_song_title = text["show_song_title"].value_or(config.show_song_title);
        config.show_artist_name = text["show_artist_name"].value_or(config.show_artist_name);
        config.show_url = text["show_url"].value_or(config.show_url);
        config.show_text_border = text["show_text_border"].value_or(config.show_text_border);
    }
    
    if (merged_config.contains("recording")) {
        const auto& recording = *merged_config["recording"].as_table();
        config.enable_recording = recording["enable_recording"].value_or(config.enable_recording);
        config.video_directory = resolve_path(recording["video_directory"].value_or(config.video_directory));
        config.video_framerate = recording["video_framerate"].value_or(config.video_framerate);
        config.ffmpeg_command = recording["ffmpeg_command"].value_or(config.ffmpeg_command);
    }

    // ... (add other sections like animation, presets, etc. in the same way)

    return true;
}