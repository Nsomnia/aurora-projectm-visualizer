// src/main.cpp
#include "core.h"
#include "Config.h"
#include "ConfigLoader.h"
#include "CliParser.h"
#include "utils/Logger.h"
#include "QtGui.h"
#include <QApplication>
#include <csignal>
#include <iostream>
#include <thread>

// Global quit flag for signal handler
volatile sig_atomic_t g_quit_flag = 0;

void signalHandler(int signum) {
    Logger::info("Caught signal " + std::to_string(signum) + ", shutting down...");
    g_quit_flag = true;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    Config config;
    if (!ConfigLoader::load(config, argv[0])) {
        Logger::error("Failed to load configuration.");
        return 1;
    }
    if (!CliParser::parse(config, argc, argv)) {
        return 0; // Exit gracefully if help was shown
    }

    Logger::set_verbose_logging(config.verbose_logging);
    if (config.verbose_logging) {
        Logger::info("Verbose logging enabled.");
        Logger::info("Configuration loaded.");
    }

    if (config.show_version) {
        Logger::info("Aurora Visualizer version " + std::string(APP_VERSION));
        return 0;
    }

    QApplication app(argc, argv);

    Core visualizerCore(config);
    if (!visualizerCore.init()) {
        Logger::error("Failed to initialize visualizer core.");
        return 1;
    }

    QtGui gui(config, visualizerCore);
    gui.init();

    // Connect the application's aboutToQuit signal to the core's cleanup method
    QObject::connect(&app, &QApplication::aboutToQuit, [&visualizerCore]() {
        visualizerCore.cleanup();
    });

    return app.exec();
}