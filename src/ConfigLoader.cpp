// src/ConfigLoader.cpp
#include "ConfigLoader.h"
#include "Config.h"
#include "utils/Logger.h"
#include "config.h"
#include <toml++/toml.h>
#include <filesystem>
#include <iostream>
#include <QString>
#include <QKeySequence>

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
    fs::path default_config_path = fs::path(CONFIG_DIR) / "default.toml";

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

    if (merged_config.contains("debug")) {
        const auto& debug = *merged_config["debug"].as_table();
        config.disable_fbo = debug["disable_fbo"].value_or(config.disable_fbo);
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

    if (merged_config.contains("presets")) {
        const auto& presets = *merged_config["presets"].as_table();
        config.presetsDirectory = resolve_path(presets["presets_directory"].value_or(config.presetsDirectory));
        config.favoritesFile = presets["favorites_file"].value_or(config.favoritesFile);
        config.shuffleEnabled = presets["shuffle_enabled"].value_or(config.shuffleEnabled);
        config.presetDuration = presets["preset_duration"].value_or(config.presetDuration);
        config.presetBlendTime = presets["preset_blend_time"].value_or(config.presetBlendTime);
        config.preset_list_file = resolve_path(presets["preset_list_file"].value_or(config.preset_list_file));
        config.broken_preset_directory = resolve_path(presets["broken_preset_directory"].value_or(config.broken_preset_directory));
        config.next_preset_key = QKeySequence::fromString(QString::fromStdString(presets["next_preset_key"].value_or("n")));
        config.prev_preset_key = QKeySequence::fromString(QString::fromStdString(presets["prev_preset_key"].value_or("p")));
        config.mark_broken_preset_key = QKeySequence::fromString(QString::fromStdString(presets["mark_broken_preset_key"].value_or("b")));
        config.favorite_preset_key = QKeySequence::fromString(QString::fromStdString(presets["favorite_preset_key"].value_or("f")));
        config.use_default_projectm_visualizer = presets["use_default_projectm_visualizer"].value_or(config.use_default_projectm_visualizer);
        config.favorites_only_shuffle = presets["favorites_only_shuffle"].value_or(config.favorites_only_shuffle);
    }

    if (merged_config.contains("audio")) {
        const auto& audio = *merged_config["audio"].as_table();
        std::string audio_mode_str = audio["audio_input_mode"].value_or("PipeWire");
        if (audio_mode_str == "SystemDefault") config.audio_input_mode = AudioInputMode::SystemDefault;
        else if (audio_mode_str == "PipeWire") config.audio_input_mode = AudioInputMode::PipeWire;
        else if (audio_mode_str == "PulseAudio") config.audio_input_mode = AudioInputMode::PulseAudio;
        else if (audio_mode_str == "File") config.audio_input_mode = AudioInputMode::File;
        else Logger::warn("Unknown audio input mode in config: " + audio_mode_str);
        config.pipewire_sink_name = audio["pipewire_sink_name"].value_or(config.pipewire_sink_name);
    }

    if (merged_config.contains("animation")) {
        const auto& animation = *merged_config["animation"].as_table();
        config.text_animation_enabled = animation["text_animation_enabled"].value_or(config.text_animation_enabled);
        config.transitionTime = animation["transition_time"].value_or(config.transitionTime);
        config.pre_fade_delay = animation["pre_fade_delay"].value_or(config.pre_fade_delay);
        config.bounce_duration = animation["bounce_duration"].value_or(config.bounce_duration);
        config.bounce_speed = animation["bounce_speed"].value_or(config.bounce_speed);
        config.bounce_randomness = animation["bounce_randomness"].value_or(config.bounce_randomness);
        config.fade_to_min_duration = animation["fade_to_min_duration"].value_or(config.fade_to_min_duration);
        config.minFadeTransparency = animation["min_fade_transparency"].value_or(config.minFadeTransparency);
        config.text_breathing_effect = animation["text_breathing_effect"].value_or(config.text_breathing_effect);
        config.breathing_effect_amount = animation["breathing_effect_amount"].value_or(config.breathing_effect_amount);
        config.breathing_effect_speed = animation["breathing_effect_speed"].value_or(config.breathing_effect_speed);
    }

    return true;
}