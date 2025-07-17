// src/main.cpp
#include "Config.h"
#include "ConfigLoader.h"
#include "CliParser.h"
#include "utils/Logger.h"
#include "ProjectMWidget.h"
#include "MainWindow.h"
#include "event_handler.h"
#include "preset_manager.h"
#include "AnimationManager.h"
#include "TextRenderer.h"
#include "TextManager.h"

#include <QApplication>
#include <QMainWindow>
#include <csignal>
#include <iostream>

// Global quit flag for signal handler
volatile sig_atomic_t g_quit_flag = 0;

void signalHandler(int signum) {
    Logger::info("Caught signal " + std::to_string(signum) + ", shutting down...");
    g_quit_flag = true;
    QApplication::quit();
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    QApplication app(argc, argv);

    Config config;
    if (!ConfigLoader::load(config, argv[0])) {
        Logger::error("Failed to load configuration.");
        return 1;
    }

    CliParser cliParser;
    cliParser.parse(argc, argv, config);
    config.postParse();

    Logger::set_verbose_logging(config.verbose_logging);
    if (config.verbose_logging) {
        Logger::info("Verbose logging enabled.");
        Logger::info("Configuration loaded.");
    }

    if (config.show_version) {
        Logger::info("Aurora Visualizer version " + std::string(APP_VERSION));
        return 0;
    }

    projectm_handle pM = projectm_create();
    projectm_set_window_size(pM, config.width, config.height);
    projectm_set_mesh_size(pM, 64, 48);
    projectm_set_soft_cut_duration(pM, config.presetBlendTime);

    PresetManager presetManager(config);
    if (!config.use_default_projectm_visualizer) {
        presetManager.load_presets(pM);
    }

    TextRenderer textRenderer;
    if (!textRenderer.init(config.font_path, config.songInfoFontSize)) {
        Logger::error("Failed to load font: " + config.font_path);
    }
    textRenderer.setProjection(config.width, config.height);

    TextManager textManager(textRenderer);
    AnimationManager animationManager(config, textRenderer);

    ProjectMWidget projectMWidget(config, pM, textRenderer, textManager, animationManager);
    MainWindow window(config, &projectMWidget);
    window.setWindowTitle("Aurora Visualizer");
    window.resize(config.width, config.height);

    EventHandler eventHandler(config, presetManager, animationManager, textRenderer, textManager, pM);
    app.installEventFilter(&eventHandler);

    window.show();

    int result = app.exec();

    projectm_destroy(pM);

    return result;
}
