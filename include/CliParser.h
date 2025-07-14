#pragma once

#include "Config.h"
#include <string>

class CliParser {
public:
    static bool parse(Config& config, int argc, char* argv[]);

private:
    static void display_help(const std::string& program_name);
};
