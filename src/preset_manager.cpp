// src/preset_manager.cpp
#include "preset_manager.h"
#include "utils/Logger.h"
#include <fstream>
#include <random>
#include <filesystem>
#include <cstdlib> // For getenv
#include <algorithm>

namespace fs = std::filesystem;

const int MAX_HISTORY_SIZE = 50; // Maximum number of presets to keep in history

PresetManager::PresetManager(const Config& config) : _config(config) {}

void PresetManager::load_presets() {
    _all_presets.clear();
    _favorite_presets.clear();
    _history.clear();
    _current_preset_index = -1;
    _history_index = -1;

    // Load all presets from the directory
    try {
        for (const auto & entry : fs::recursive_directory_iterator(_config.presetsDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".milk") {
                _all_presets.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error reading presets directory: " << e.what() << std::endl;
    }

    // Load favorites
    load_favorites();

    // Select an initial random preset if available
    if (!_all_presets.empty()) {
        std::string initial_preset = get_random_preset(_all_presets);
        _history.push_back(initial_preset);
        _history_index = 0;
    }
}

std::string PresetManager::get_next_preset() {
    if (_all_presets.empty()) {
        return "";
    }

    // If we've gone back in history, clear the "future" history before adding a new preset
    if (_history_index != -1 && _history_index < (int)_history.size() - 1) {
        _history.erase(_history.begin() + _history_index + 1, _history.end());
    }

    std::string preset = get_random_preset(_all_presets);
    _history.push_back(preset);
    _history_index++;

    // Cap history size
    if (_history.size() > MAX_HISTORY_SIZE) {
        _history.erase(_history.begin());
        _history_index--; // Adjust index as the first element was removed
    }
    return preset;
}

std::string PresetManager::get_prev_preset() {
    if (_history_index > 0) { // Can go back if not at the very first element
        _history_index--;
        return _history[_history_index];
    }
    return ""; // Cannot go back further
}

std::string PresetManager::get_current_preset() const {
    if (_history_index >= 0 && _history_index < (int)_history.size()) {
        return _history[_history_index];
    }
    return "";
}


void PresetManager::mark_current_preset_as_broken() {
    std::string current_preset = get_current_preset();
    if (current_preset.empty()) {
        return;
    }

    try {
        fs::path source_path(current_preset);
        
        std::string raw_broken_dir = _config.broken_preset_directory;
        fs::path broken_dir_path;
        if (raw_broken_dir.rfind("~", 0) == 0) {
            const char* home_dir = getenv("HOME");
            if (home_dir) {
                std::string path_without_tilde = raw_broken_dir.substr(1);
                if (!path_without_tilde.empty() && path_without_tilde[0] == fs::path::preferred_separator) {
                    path_without_tilde = path_without_tilde.substr(1);
                }
                broken_dir_path = fs::path(home_dir) / path_without_tilde;
            } else {
                Logger::warn("HOME environment variable not set. Cannot resolve broken preset directory: " + raw_broken_dir);
                broken_dir_path = raw_broken_dir;
            }
        } else {
            broken_dir_path = raw_broken_dir;
        }

        fs::create_directories(broken_dir_path);
        fs::path dest_path = broken_dir_path / source_path.filename();

        fs::rename(source_path, dest_path);

        std::cout << "Moved broken preset to: " << dest_path << std::endl;

        // Remove from all lists
        _all_presets.erase(std::remove(_all_presets.begin(), _all_presets.end(), current_preset), _all_presets.end());
        _favorite_presets.erase(std::remove(_favorite_presets.begin(), _favorite_presets.end(), current_preset), _favorite_presets.end());
        _history.erase(std::remove(_history.begin(), _history.end(), current_preset), _history.end());
        _history_index--;


    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error moving preset file: " << e.what() << std::endl;
    }
}

void PresetManager::toggle_favorite_current_preset() {
    std::string current_preset = get_current_preset();
    if (current_preset.empty()) {
        return;
    }

    auto it = std::find(_favorite_presets.begin(), _favorite_presets.end(), current_preset);
    if (it == _favorite_presets.end()) {
        // Add to favorites
        _favorite_presets.push_back(current_preset);
        std::cout << "Added to favorites: " << current_preset << std::endl;
    } else {
        // Remove from favorites
        _favorite_presets.erase(it);
        std::cout << "Removed from favorites: " << current_preset << std::endl;
    }
    save_favorites();
}


void PresetManager::load_favorites() {
    _favorite_presets.clear();
    std::string raw_path = _config.favoritesFile;
    fs::path resolved_path;

    if (raw_path.rfind("~", 0) == 0) {
        const char* home_dir = getenv("HOME");
        if (home_dir) {
            std::string path_without_tilde = raw_path.substr(1);
            if (!path_without_tilde.empty() && path_without_tilde[0] == fs::path::preferred_separator) {
                path_without_tilde = path_without_tilde.substr(1);
            }
            resolved_path = fs::path(home_dir) / path_without_tilde;
        } else {
            Logger::warn("HOME environment variable not set. Cannot resolve path: " + raw_path);
            resolved_path = raw_path;
        }
    } else {
        resolved_path = raw_path;
    }

    if (!fs::exists(resolved_path)) {
        return; // No favorites file yet
    }

    std::ifstream favorites_file(resolved_path.string());
    if (!favorites_file.is_open()) {
        Logger::error("Could not open favorites file: " + resolved_path.string());
        return;
    }

    std::string line;
    while (std::getline(favorites_file, line)) {
        if (!line.empty()) {
            _favorite_presets.push_back(line);
        }
    }
}

void PresetManager::save_favorites() {
     std::string raw_path = _config.favoritesFile;
    fs::path resolved_path;

    if (raw_path.rfind("~", 0) == 0) {
        const char* home_dir = getenv("HOME");
        if (home_dir) {
            std::string path_without_tilde = raw_path.substr(1);
            if (!path_without_tilde.empty() && path_without_tilde[0] == fs::path::preferred_separator) {
                path_without_tilde = path_without_tilde.substr(1);
            }
            resolved_path = fs::path(home_dir) / path_without_tilde;
        } else {
            Logger::warn("HOME environment variable not set. Cannot resolve path: " + raw_path);
            resolved_path = raw_path;
        }
    } else {
        resolved_path = raw_path;
    }

    std::ofstream favorites_file(resolved_path.string());
    if (!favorites_file.is_open()) {
        Logger::error("Could not open favorites file for writing: " + resolved_path.string());
        return;
    }

    for (const auto& preset : _favorite_presets) {
        favorites_file << preset << std::endl;
    }
}

std::string PresetManager::get_random_preset(const std::vector<std::string>& preset_list) {
    if (preset_list.empty()) {
        return "";
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, preset_list.size() - 1);

    _current_preset_index = distrib(gen);
    return preset_list[_current_preset_index];
}