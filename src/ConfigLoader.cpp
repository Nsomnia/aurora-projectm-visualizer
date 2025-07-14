// src/ConfigLoader.cpp
#include "ConfigLoader.h"
#include "Config.h"
#include "utils/Logger.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <unordered_map>

namespace fs = std::filesystem;

// Helper function to trim leading/trailing whitespace
static std::string trim(const std::string &str) {
  const auto strBegin = str.find_first_not_of(" \t");
  if (strBegin == std::string::npos)
    return "";
  const auto strEnd = str.find_last_not_of(" \t");
  const auto strRange = strEnd - strBegin + 1;
  return str.substr(strBegin, strRange);
}

// Helper function to remove surrounding quotes
static std::string remove_quotes(const std::string &str) {
  if (str.length() >= 2 && (str.front() == '"' || str.front() == '\'') &&
      (str.back() == '"' || str.back() == '\'')) {
    return str.substr(1, str.length() - 2);
  }
  return str;
}

bool ConfigLoader::load(Config& config, const std::string &executable_path_str) {
    fs::path executable_path(executable_path_str);
    fs::path config_path;

    // 1. Check user-specific config first
    const char *home_dir = getenv("HOME");
    if (home_dir) {
        fs::path user_config_path = fs::path(home_dir) / ".config" / "aurora-visualizer" / "config.toml";
        if (fs::exists(user_config_path)) {
            config_path = user_config_path;
        }
    }

    // 2. If no user config, find the default config relative to the project root
    if (config_path.empty()) {
        fs::path current_path = fs::absolute(executable_path).parent_path();
        fs::path project_root;

        // Traverse up to find a known file/directory that marks the project root
        while (!current_path.empty() && current_path != current_path.root_path()) {
            if (fs::exists(current_path / "config") && fs::exists(current_path / "src")) {
                project_root = current_path;
                break;
            }
            current_path = current_path.parent_path();
        }

        if (!project_root.empty()) {
            config_path = project_root / "config" / "default.toml";
        } else {
            std::cerr << "Warning: Could not find project root. Unable to load default config." << std::endl;
            // As a last resort, you might try a path relative to the executable
            config_path = fs::absolute(executable_path).parent_path().parent_path() / "config" / "default.toml";
        }
    }

    if (!config_path.empty() && fs::exists(config_path)) {
        load_from_file(config, config_path.string());
    } else {
        std::cerr << "Warning: No configuration file found. Using default values." << std::endl;
    }

    return true;
}

void ConfigLoader::load_from_file(Config& config, const std::string &path) {
    std::ifstream configFile(path);
    if (!configFile.is_open()) {
        std::cerr << "Warning: Could not open config file " << path
                  << ". Using defaults." << std::endl;
        return;
    }

    std::unordered_map<std::string, std::function<void(const std::string&)>> parsers;
    parsers["resolution_width"] = [&config](const std::string& v){ config.width = std::stoi(v); };
    parsers["resolution_height"] = [&config](const std::string& v){ config.height = std::stoi(v); };
    parsers["fps"] = [&config](const std::string& v){ config.fps = std::stoi(v); };
    parsers["font_path"] = [&config](const std::string& v){ config.font_path = v; };
    parsers["presets_directory"] = [&config](const std::string& v){ config.presetsDirectory = v; };
    parsers["favorites_file"] = [&config](const std::string& v){ config.favoritesFile = v; };
    parsers["shuffle_enabled"] = [&config](const std::string& v){ config.shuffleEnabled = (v == "true"); };
    parsers["preset_duration"] = [&config](const std::string& v){ config.presetDuration = std::stod(v); };
    parsers["preset_blend_time"] = [&config](const std::string& v){ config.presetBlendTime = std::stod(v); };
    parsers["url_text"] = [&config](const std::string& v){ config.urlText = v; };
    parsers["url_font_size"] = [&config](const std::string& v){ config.urlFontSize = std::stoi(v); };
    parsers["url_font_color"] = [&config](const std::string& v){ config.urlFontColor = hexToVec3(v); };
    parsers["url_border_color"] = [&config](const std::string& v){ config.urlBorderColor = hexToVec3(v); };
    parsers["url_border_thickness"] = [&config](const std::string& v){ config.urlBorderThickness = std::stof(v); };
    parsers["artist_name"] = [&config](const std::string& v){ config.artistName = v; };
    parsers["show_song_title"] = [&config](const std::string& v){ config.show_song_title = (v == "true"); };
    parsers["show_artist_name"] = [&config](const std::string& v){ config.show_artist_name = (v == "true"); };
    parsers["show_url"] = [&config](const std::string& v){ config.show_url = (v == "true"); };
    parsers["song_info_font_size"] = [&config](const std::string& v){ config.songInfoFontSize = std::stoi(v); };
    parsers["song_info_font_color"] = [&config](const std::string& v){ config.songInfoFontColor = hexToVec3(v); };
    parsers["song_info_border_color"] = [&config](const std::string& v){ config.songInfoBorderColor = hexToVec3(v); };
    parsers["song_info_border_thickness"] = [&config](const std::string& v){ config.songInfoBorderThickness = std::stof(v); };
    parsers["show_text_border"] = [&config](const std::string& v){ config.show_text_border = (v == "true"); };
    parsers["text_animation_enabled"] = [&config](const std::string& v){ config.text_animation_enabled = (v == "true"); };
    parsers["transition_fade_time"] = [&config](const std::string& v){ config.transitionTime = std::stof(v); };
    parsers["pre_fade_delay"] = [&config](const std::string& v){ config.pre_fade_delay = std::stof(v); };
    parsers["bounce_duration"] = [&config](const std::string& v){ config.bounce_duration = std::stof(v); };
    parsers["bounce_speed"] = [&config](const std::string& v){ config.bounce_speed = std::stof(v); };
    parsers["bounce_randomness"] = [&config](const std::string& v){ config.bounce_randomness = std::stof(v); };
    parsers["fade_to_min_duration"] = [&config](const std::string& v){ config.fade_to_min_duration = std::stof(v); };
    parsers["min_fade_transparency"] = [&config](const std::string& v){ config.minFadeTransparency = std::stof(v); };
    parsers["text_breathing_effect"] = [&config](const std::string& v){ config.text_breathing_effect = (v == "true"); };
    parsers["breathing_effect_amount"] = [&config](const std::string& v){ config.breathing_effect_amount = std::stof(v); };
    parsers["breathing_effect_speed"] = [&config](const std::string& v){ config.breathing_effect_speed = std::stof(v); };
    parsers["record_video"] = [&config](const std::string& v){ config.enable_recording = (v == "true"); };
    parsers["output_directory"] = [&config](const std::string& v){ config.video_directory = v; };
    parsers["video_framerate"] = [&config](const std::string& v){ config.video_framerate = std::stoi(v); };
    parsers["ffmpeg_command"] = [&config](const std::string& v){ strncpy(config.ffmpeg_command, v.c_str(), sizeof(config.ffmpeg_command) - 1); config.ffmpeg_command[sizeof(config.ffmpeg_command) - 1] = '\0'; };
    parsers["preset_list_file"] = [&config](const std::string& v){ config.preset_list_file = v; };
    parsers["broken_preset_directory"] = [&config](const std::string& v){ config.broken_preset_directory = v; };
    parsers["next_preset_key"] = [&config](const std::string& v){ config.next_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["prev_preset_key"] = [&config](const std::string& v){ config.prev_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["mark_broken_preset_key"] = [&config](const std::string& v){ config.mark_broken_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["favorite_preset_key"] = [&config](const std::string& v){ config.favorite_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["use_default_projectm_visualizer"] = [&config](const std::string& v){ config.use_default_projectm_visualizer = (v == "true"); };

    std::string line;
    while (std::getline(configFile, line)) {
        auto commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line = trim(line);
        if (line.empty()) {
            continue;
        }
        auto delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = trim(line.substr(0, delimiterPos));
            std::string value = trim(line.substr(delimiterPos + 1));
            value = remove_quotes(value);

            auto it = parsers.find(key);
            if (it != parsers.end()) {
                if (key == "ffmpeg_command" && value.length() > 1024) {
                    Logger::warn("ffmpeg_command is too long. Truncating to 1024 characters.");
                    value = value.substr(0, 1024);
                }
                it->second(value);
            }
        }
    }
}