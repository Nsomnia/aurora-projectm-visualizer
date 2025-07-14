#pragma once

#include "Config.h"
#include <string>
#include <vector>

class PresetManager {
public:
    PresetManager(const Config& config);

    void load_presets();
    std::string get_next_preset();
    std::string get_prev_preset();
    std::string get_current_preset() const;

    void mark_current_preset_as_broken();
    void toggle_favorite_current_preset();


private:
    void load_favorites();
    void save_favorites();
    std::string get_random_preset(const std::vector<std::string>& preset_list);


    const Config& _config;
    std::vector<std::string> _all_presets;
    std::vector<std::string> _favorite_presets;
    std::vector<std::string> _history;
    int _current_preset_index = -1;
    int _history_index = -1;
};
