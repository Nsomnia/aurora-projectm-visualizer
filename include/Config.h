// include/visualizer/Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <QKeySequence>

// Define APP_VERSION if not already defined (e.g., by CMake)
#ifndef APP_VERSION
#define APP_VERSION "0.1.0-dev"
#endif

enum class AudioInputMode {
    SystemDefault,
    PipeWire,
    PulseAudio,
    File
};

struct Config {
    // Display
    int width = 1024;
    int height = 640;
    int fps = 30;

    // Font & Text
    std::string font_path = "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf";
    std::string songTitle = "No Song Title";
    std::string artistName = "Unknown Artist";
    std::string urlText = "https://www.youtube.com/@theaudiovisualgod";
    int songInfoFontSize = 42;
    int urlFontSize = 28;
    glm::vec3 songInfoFontColor{1.0f, 1.0f, 1.0f};
    glm::vec3 urlFontColor{0.0f, 0.639f, 0.694f};
    glm::vec3 songInfoBorderColor{0.0f, 0.0f, 0.0f};
    glm::vec3 urlBorderColor{0.0f, 0.0f, 0.0f};
    float songInfoBorderThickness = 0.2f;
    float urlBorderThickness = 0.2f;
    bool show_text_border = true;
    bool show_song_title = true;
    bool show_artist_name = true;
    bool show_url = true;

    // Text Animation
    bool text_animation_enabled = true;
    float transitionTime = 10.0f;
    float pre_fade_delay = 10.0f;
    float bounce_duration = 7.0f;
    float bounce_speed = 67.0f;
    float bounce_randomness = 5.0f;
    float fade_to_min_duration = 10.0f;
    float minFadeTransparency = 0.15f;
    bool text_breathing_effect = true;
    float breathing_effect_amount = 0.05f;
    float breathing_effect_speed = 1.0f;

    // Presets
    std::string presetsDirectory = "/usr/share/projectM/presets";
    std::string favoritesFile = "favorites.txt";
    bool shuffleEnabled = true;
    double presetDuration = 15.0;
    double presetBlendTime = 2.7;
    std::string preset_list_file;
    std::string broken_preset_directory = "broken_presets/";
    QKeySequence next_preset_key = QKeySequence(Qt::Key_N);
    QKeySequence prev_preset_key = QKeySequence(Qt::Key_P);
    QKeySequence mark_broken_preset_key = QKeySequence(Qt::Key_B);
    QKeySequence favorite_preset_key = QKeySequence(Qt::Key_F);
    bool use_default_projectm_visualizer = false;
    bool favorites_only_shuffle = false;

    // Recording
    bool enable_recording = false;
    std::string video_directory = "videos";
    int video_framerate = 24;
    std::string ffmpeg_command = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s {WIDTH}x{HEIGHT} -framerate {FPS} -i - -i {AUDIO_FILE_PATH} -c:v libx265 -crf 28 -preset medium -c:a copy {OUTPUT_PATH}";

    // Audio
    std::vector<std::string> audio_file_paths;
    AudioInputMode audio_input_mode = AudioInputMode::PipeWire;
    std::string pipewire_sink_name = "AuroraSink";

    // Other
    bool show_version = false;
    bool verbose_logging = false;

    // CLI only
    std::string songInfoFontColorStr;
    std::string urlFontColorStr;
    std::string songInfoBorderColorStr;
    std::string urlBorderColorStr;
    std::string audio_input_mode_str;
    std::string next_preset_key_str;
    std::string prev_preset_key_str;
    std::string mark_broken_preset_key_str;
    std::string favorite_preset_key_str;

    void postParse() {
        if (!songInfoFontColorStr.empty()) songInfoFontColor = hexToVec3(songInfoFontColorStr);
        if (!urlFontColorStr.empty()) urlFontColor = hexToVec3(urlFontColorStr);
        if (!songInfoBorderColorStr.empty()) songInfoBorderColor = hexToVec3(songInfoBorderColorStr);
        if (!urlBorderColorStr.empty()) urlBorderColor = hexToVec3(urlBorderColorStr);

        if (audio_input_mode_str == "SystemDefault") audio_input_mode = AudioInputMode::SystemDefault;
        else if (audio_input_mode_str == "PipeWire") audio_input_mode = AudioInputMode::PipeWire;
        else if (audio_input_mode_str == "PulseAudio") audio_input_mode = AudioInputMode::PulseAudio;
        else if (audio_input_mode_str == "File") audio_input_mode = AudioInputMode::File;

        if (!next_preset_key_str.empty()) next_preset_key = QKeySequence(next_preset_key_str);
        if (!prev_preset_key_str.empty()) prev_preset_key = QKeySequence(prev_preset_key_str);
        if (!mark_broken_preset_key_str.empty()) mark_broken_preset_key = QKeySequence(mark_broken_preset_key_str);
        if (!favorite_preset_key_str.empty()) favorite_preset_key = QKeySequence(favorite_preset_key_str);
    }
};

// Utility function to convert hex color string to glm::vec3
inline glm::vec3 hexToVec3(std::string hex) {
    if (hex.length() == 6) {
        unsigned int r, g, b;
        sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
        return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
    }
    return glm::vec3(1.0f, 1.0f, 1.0f); // Default to white
}

#endif // CONFIG_H


