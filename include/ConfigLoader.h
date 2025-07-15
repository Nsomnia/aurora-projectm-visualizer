#pragma once

#include "Config.h"
#include <string>

class ConfigLoader {
public:
    static bool load(Config& config, const std::string &executable_path_str);
};
