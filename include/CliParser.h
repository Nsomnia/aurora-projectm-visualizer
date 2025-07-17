#pragma once

#include "Config.h"

namespace CLI {
    class App;
}

class CliParser {
public:
    CliParser();
    void parse(int argc, char** argv, Config& config);
    void add_options(CLI::App& app, Config& config);

private:
    std::shared_ptr<CLI::App> app;
};
