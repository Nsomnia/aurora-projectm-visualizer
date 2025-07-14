#pragma once

#include "Config.h"
#include <string>

class ConfigLoader {
public:
    static bool load(Config& config, const std::string& executable_path);

private:
    static void load_from_file(Config& config, const std::string& path);
};
