#include "CliParser.h"
#include "Config.h"
#include "CLI/CLI.hpp"
#include <iostream>
#include <string>

CliParser::CliParser() {
    app = std::make_shared<CLI::App>("Aurora Visualizer");
}

void CliParser::parse(int argc, char** argv, Config& config) {
    add_options(*app, config);
    try {
        app->parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        exit(app->exit(e));
    }
}

void CliParser::add_options(CLI::App& app, Config& config) {
    app.add_option("--width", config.width, "Set window width");
    app.add_option("--height", config.height, "Set window height");
    app.add_option("--fps", config.fps, "Set frames per second");
    app.add_option("--font-path", config.font_path, "Path to the font file (TTF/OTF)");
    app.add_option("--song-info-font-size", config.songInfoFontSize, "Font size for song title and artist");
    app.add_option("--url-font-size", config.urlFontSize, "Font size for the URL text");
    app.add_option("--song-info-color", config.songInfoFontColorStr, "Font color for song info in hexadecimal (e.g., FFFFFF)");
    app.add_option("--url-color", config.urlFontColorStr, "Font color for URL in hexadecimal (e.g., 00A3B1)");
    app.add_option("--song-info-border-color", config.songInfoBorderColorStr, "Color for the song info text border in hexadecimal");
    app.add_option("--url-border-color", config.urlBorderColorStr, "Color for the URL text border in hexadecimal");
    app.add_option("--song-info-border-thickness", config.songInfoBorderThickness, "Thickness of the song info text border");
    app.add_option("--url-border-thickness", config.urlBorderThickness, "Thickness of the URL text border");
    app.add_option("--title", config.songTitle, "Custom text to display as the song title");
    app.add_option("--artist-name", config.artistName, "Set custom artist name overlay");
    app.add_option("--url-text", config.urlText, "Set custom URL text overlay");
    app.add_flag("--hide-title", config.show_song_title, "Hide the song title text overlay");
    app.add_flag("--hide-artist", config.show_artist_name, "Hide the artist name text overlay");
    app.add_flag("--hide-url", config.show_url, "Hide the URL text overlay");
    app.add_flag("--disable-text-animation", config.text_animation_enabled, "Disable all text animation effects");
    app.add_option("--transition-time", config.transitionTime, "Duration of the fade effect at the end of text display");
    app.add_option("--pre-fade-delay", config.pre_fade_delay, "Time to wait at full opacity before text starts fading out");
    app.add_option("--bounce-duration", config.bounce_duration, "How long the text bounces before fading");
    app.add_option("--bounce-speed", config.bounce_speed, "Speed of the bouncing text");
    app.add_option("--fade-to-min-duration", config.fade_to_min_duration, "Time it takes for text to fade to its minimum transparency");
    app.add_option("--min-transparency", config.minFadeTransparency, "The minimum transparency for the text");
    app.add_option("--preset-list-file", config.preset_list_file, "Path to a file containing a list of .milk presets to load");
    app.add_option("--broken-preset-directory", config.broken_preset_directory, "Directory to move broken presets to");
    app.add_option("--favorites-file", config.favoritesFile, "Path to the favorites file");
    app.add_option("--next-preset-key", config.next_preset_key_str, "Key to load the next random preset (e.g., n)");
    app.add_option("--prev-preset-key", config.prev_preset_key_str, "Key to load the previous preset (e.g., p)");
    app.add_option("--mark-broken-preset-key", config.mark_broken_preset_key_str, "Key to mark the current preset as broken (e.g., b)");
    app.add_option("--favorite-preset-key", config.favorite_preset_key_str, "Key to mark the current preset as a favorite (e.g., f)");
    app.add_flag("--record-video", config.enable_recording, "Enable video recording");
    app.add_option("--audio-input-mode", config.audio_input_mode_str, "Set audio input mode for recording. Options: SystemDefault, PipeWire, PulseAudio, File");
    app.add_option("--pipewire-sink-name", config.pipewire_sink_name, "Set the name of the virtual PipeWire sink to create");
    app.add_option("--output-directory", config.video_directory, "Directory to save recorded videos");
    app.add_option("--video-framerate", config.video_framerate, "Set video recording framerate");
    app.add_option("--ffmpeg-command", config.ffmpeg_command, "The FFmpeg command template for recording");
    app.add_option("--audio-file", config.audio_file_paths, "Add an audio file to the playlist. Can be used multiple times");
    app.add_flag("--version", config.show_version, "Display application version");
    app.add_flag("--verbose", config.verbose_logging, "Enable verbose logging");
}
