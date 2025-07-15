// src/CliParser.cpp
#include "CliParser.h"
#include "Config.h"
#include "utils/Logger.h"
#include <vector>
#include <functional>
#include <unordered_map>

void CliParser::display_help(const std::string &program_name) {
  // ANSI escape codes for colors
  const std::string RESET = "\033[0m";
  const std::string BOLD = "\033[1m";
  const std::string GREEN = "\033[32m";
  const std::string YELLOW = "\033[33m";
  const std::string CYAN = "\033[36m";
  const std::string MAGENTA = "\033[35m";

  std::cout << BOLD << GREEN << "Aurora Visualizer v" << APP_VERSION << RESET << "\n\n"
            << BOLD << YELLOW << "Usage: " << program_name << RESET
            << " [options] [audio_file_1 audio_file_2 ...]\n\n"

            << BOLD << MAGENTA << "Display & Performance" << RESET << "\n"
            << "  " << BOLD << GREEN << "--width <px>" << RESET << "               Set window width (default: 1024).\n"
            << "  " << BOLD << GREEN << "--height <px>" << RESET << "              Set window height (default: 640).\n"
            << "  " << BOLD << GREEN << "--fps <value>" << RESET << "              Set frames per second (default: 30).\n\n"

            << BOLD << MAGENTA << "Text & Font" << RESET << "\n"
            << "  " << BOLD << GREEN << "--font-path <path>" << RESET << "         Path to the font file (TTF/OTF).\n"
            << "  " << BOLD << GREEN << "--song-info-font-size <pt>" << RESET << " Font size for song title and artist.\n"
            << "  " << BOLD << GREEN << "--url-font-size <pt>" << RESET << "       Font size for the URL text.\n"
            << "  " << BOLD << GREEN << "--song-info-color <hex>" << RESET << "    Font color for song info (e.g., FFFFFF).\n"
            << "  " << BOLD << GREEN << "--url-color <hex>" << RESET << "          Font color for URL (e.g., 00A3B1).\n"
            << "  " << BOLD << GREEN << "--song-info-border-color <hex>" << RESET << " Color for the song info text border.\n"
            << "  " << BOLD << GREEN << "--url-border-color <hex>" << RESET << "   Color for the URL text border.\n"
            << "  " << BOLD << GREEN << "--song-info-border-thickness <val>" << RESET << " Thickness of the song info text border.\n"
            << "  " << BOLD << GREEN << "--url-border-thickness <val>" << RESET << "   Thickness of the URL text border.\n\n"

            << BOLD << MAGENTA << "Text Content & Visibility" << RESET << "\n"
            << "  " << BOLD << GREEN << "--title <text>" << RESET << "             Custom text to display as the title.\n"
            << "  " << BOLD << GREEN << "--artist-name <name>" << RESET << "       Set artist name overlay.\n"
            << "  " << BOLD << GREEN << "--url-text <text>" << RESET << "          Set URL text overlay.\n"
            << "  " << BOLD << GREEN << "--hide-title" << RESET << "               Hide the song title text.\n"
            << "  " << BOLD << GREEN << "--hide-artist" << RESET << "              Hide the artist name text.\n"
            << "  " << BOLD << GREEN << "--hide-url" << RESET << "                 Hide the URL text.\n"
            << "  " << BOLD << GREEN << "--hide-border" << RESET << "              Hide the text border.\n\n"

            << BOLD << MAGENTA << "Animation" << RESET << "\n"
            << "  " << BOLD << GREEN << "--disable-text-animation" << RESET << "   Disable the text animation.\n"
            << "  " << BOLD << GREEN << "--transition-time <sec>" << RESET << "    Duration of the fade effect at the end.\n"
            << "  " << BOLD << GREEN << "--pre-fade-delay <sec>" << RESET << "     Time to bounce at full opacity before fading.\n"
            << "  " << BOLD << GREEN << "--bounce-duration <sec>" << RESET << "    How long the text bounces before fading.\n"
            << "  " << BOLD << GREEN << "--bounce-speed <pixels/sec>" << RESET << " Speed of the bouncing text.\n"
            << "  " << BOLD << GREEN << "--bounce-randomness <val>" << RESET << "  Amount of randomness to apply to bounce angle.\n"
            << "  " << BOLD << GREEN << "--fade-to-min-duration <sec>" << RESET << " Time it takes to fade to minimum transparency.\n"
            << "  " << BOLD << GREEN << "--min-transparency <0-1>" << RESET << "   The minimum transparency for the text.\n"
            << "  " << BOLD << GREEN << "--disable-breathing-effect" << RESET << " Disable the text breathing effect.\n"
            << "  " << BOLD << GREEN << "--breathing-amount <val>" << RESET << "   The amount the text size changes for the breathing effect.\n"
            << "  " << BOLD << GREEN << "--breathing-speed <val>" << RESET << "    The speed of the text breathing effect.\n\n"

            << BOLD << MAGENTA << "Presets" << RESET << "\n"
            << "  " << BOLD << GREEN << "--preset-duration <sec>" << RESET << "    Time before switching to the next preset.\n"
            << "  " << BOLD << GREEN << "--preset-blend-time <sec>" << RESET << "  Time for the blend transition between presets.\n"
            << "  " << BOLD << GREEN << "--preset-list-file <path>" << RESET << "  Path to a file containing a list of .milk presets.\n"
            << "  " << BOLD << GREEN << "--broken-preset-directory <path>" << RESET << " Directory to move broken presets to.\n"
            << "  " << BOLD << GREEN << "--favorites-file <path>" << RESET << "    Path to the favorites file.\n"
            << "  " << BOLD << GREEN << "--next-preset-key <key>" << RESET << "    Key to load the next random preset (e.g., 'n').\n"
            << "  " << BOLD << GREEN << "--prev-preset-key <key>" << RESET << "    Key to load the previous preset (e.g., 'p').\n"
            << "  " << BOLD << GREEN << "--mark-broken-preset-key <key>" << RESET << " Key to mark the current preset as broken (e.g., 'b').\n"
            << "  " << BOLD << GREEN << "--favorite-preset-key <key>" << RESET << " Key to mark the current preset as a favorite (e.g., 'f').\n"
            << "  " << BOLD << GREEN << "--use-default-projectm-visualizer" << RESET << " Use projectM's default visualizer for testing audio input.\n"
            << "  " << BOLD << GREEN << "--favorites-only-shuffle" << RESET << "    Only shuffle favorite presets.\n\n"

            << BOLD << MAGENTA << "Recording" << RESET << "\n"
            << "  " << BOLD << GREEN << "--record-video" << RESET << "             Enable video recording.\n"
            << "  " << BOLD << GREEN << "--audio-input-mode <mode>" << RESET << "  Set audio input mode (SystemDefault, PipeWire, PulseAudio, File). Default: PipeWire.\n"
            << "  " << BOLD << GREEN << "--pipewire-sink-name <name>" << RESET << " Set the name of the virtual PipeWire sink (default: AuroraSink).\n"
            << "  " << BOLD << GREEN << "--output-directory <path>" << RESET << "  Directory to save recorded videos.\n"
            << "  " << BOLD << GREEN << "--video-framerate <value>" << RESET << "  Set video recording framerate.\n"
            << "  " << BOLD << GREEN << "--ffmpeg-command <cmd>" << RESET << "     The ffmpeg command template for recording.\n\n"

            << BOLD << MAGENTA << "Other" << RESET << "\n"
            << "  " << BOLD << GREEN << "--audio-file <path>" << RESET << "        Add an audio file to the playlist (can be used multiple times).\n"
            << "  " << BOLD << GREEN << "--version" << RESET << "                  Display application version.\n"
            << "  " << BOLD << GREEN << "--verbose" << RESET << "                  Enable verbose logging.\n"
            << "  " << BOLD << GREEN << "-h, --help" << RESET << "                 Display this help message.\n";
}

bool CliParser::parse(Config& config, int argc, char *argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    if (args.empty() && argc > 1) { // No arguments, but program was run
        return true;
    }

    for (const auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            display_help(argv[0]);
            return false;
        }
    }

    std::unordered_map<std::string, std::function<void(const std::string&)>> parsers;
    parsers["--width"] = [&config](const std::string& v){ config.width = std::stoi(v); };
    parsers["--height"] = [&config](const std::string& v){ config.height = std::stoi(v); };
    parsers["--fps"] = [&config](const std::string& v){ config.fps = std::stoi(v); };
    parsers["--output-directory"] = [&config](const std::string& v){ config.video_directory = v; };
    parsers["--video-framerate"] = [&config](const std::string& v){ config.video_framerate = std::stoi(v); };
    parsers["--ffmpeg-command"] = [&config](const std::string& v){ config.ffmpeg_command = v; };
    parsers["--preset-duration"] = [&config](const std::string& v){ config.presetDuration = std::stod(v); };
    parsers["--preset-blend-time"] = [&config](const std::string& v){ config.presetBlendTime = std::stod(v); };
    parsers["--preset-list-file"] = [&config](const std::string& v){ config.preset_list_file = v; };
    parsers["--broken-preset-directory"] = [&config](const std::string& v){ config.broken_preset_directory = v; };
    parsers["--favorites-file"] = [&config](const std::string& v){ config.favoritesFile = v; };
    parsers["--next-preset-key"] = [&config](const std::string& v){ config.next_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--prev-preset-key"] = [&config](const std::string& v){ config.prev_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--mark-broken-preset-key"] = [&config](const std::string& v){ config.mark_broken_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--favorite-preset-key"] = [&config](const std::string& v){ config.favorite_preset_key = SDL_GetKeyFromName(v.c_str()); };
    parsers["--url-text"] = [&config](const std::string& v){ config.urlText = v; };
    parsers["--artist-name"] = [&config](const std::string& v){ config.artistName = v; };
    parsers["--font-path"] = [&config](const std::string& v){ config.font_path = v; };
    parsers["--audio-file"] = [&config](const std::string& v){ config.audio_file_paths.push_back(v); };
    parsers["--title"] = [&config](const std::string& v){ config.songTitle = v; };
    parsers["--song-info-font-size"] = [&config](const std::string& v){ config.songInfoFontSize = std::stoi(v); };
    parsers["--url-font-size"] = [&config](const std::string& v){ config.urlFontSize = std::stoi(v); };
    parsers["--song-info-color"] = [&config](const std::string& v){ config.songInfoFontColor = hexToVec3(v); };
    parsers["--url-color"] = [&config](const std::string& v){ config.urlFontColor = hexToVec3(v); };
    parsers["--song-info-border-color"] = [&config](const std::string& v){ config.songInfoBorderColor = hexToVec3(v); };
    parsers["--url-border-color"] = [&config](const std::string& v){ config.urlBorderColor = hexToVec3(v); };
    parsers["--border-thickness"] = [&config](const std::string& v){ config.urlBorderThickness = std::stof(v); };
    parsers["--song-info-border-thickness"] = [&config](const std::string& v){ config.songInfoBorderThickness = std::stof(v); };
    parsers["--url-border-thickness"] = [&config](const std::string& v){ config.urlBorderThickness = std::stof(v); };
    parsers["--transition-time"] = [&config](const std::string& v){ config.transitionTime = std::stof(v); };
    parsers["--pre-fade-delay"] = [&config](const std::string& v){ config.pre_fade_delay = std::stof(v); };
    parsers["--bounce-duration"] = [&config](const std::string& v){ config.bounce_duration = std::stof(v); };
    parsers["--bounce-speed"] = [&config](const std::string& v){ config.bounce_speed = std::stof(v); };
    parsers["--bounce-randomness"] = [&config](const std::string& v){ config.bounce_randomness = std::stof(v); };
    parsers["--fade-to-min-duration"] = [&config](const std::string& v){ config.fade_to_min_duration = std::stof(v); };
    parsers["--min-transparency"] = [&config](const std::string& v){ config.minFadeTransparency = std::stof(v); };
    parsers["--breathing-amount"] = [&config](const std::string& v){ config.breathing_effect_amount = std::stof(v); };
    parsers["--breathing-speed"] = [&config](const std::string& v){ config.breathing_effect_speed = std::stof(v); };
    parsers["--audio-input-mode"] = [&config](const std::string& v){
        if (v == "SystemDefault") config.audio_input_mode = AudioInputMode::SystemDefault;
        else if (v == "PipeWire") config.audio_input_mode = AudioInputMode::PipeWire;
        else if (v == "PulseAudio") config.audio_input_mode = AudioInputMode::PulseAudio;
        else if (v == "File") config.audio_input_mode = AudioInputMode::File;
        else std::cerr << "Unknown audio input mode: " << v << std::endl;
    };
    parsers["--pipewire-sink-name"] = [&config](const std::string& v){ config.pipewire_sink_name = v; };

    std::unordered_map<std::string, std::function<void()>> flag_parsers;
    flag_parsers["--record-video"] = [&config](){ config.enable_recording = true; };
    flag_parsers["--disable-text-animation"] = [&config](){ config.text_animation_enabled = false; };
    flag_parsers["--hide-title"] = [&config](){ config.show_song_title = false; };
    flag_parsers["--hide-artist"] = [&config](){ config.show_artist_name = false; };
    flag_parsers["--hide-url"] = [&config](){ config.show_url = false; };
    flag_parsers["--hide-border"] = [&config](){ config.show_text_border = false; };
    flag_parsers["--disable-breathing-effect"] = [&config](){ config.text_breathing_effect = false; };
    flag_parsers["--version"] = [&config](){ config.show_version = true; };
    flag_parsers["--verbose"] = [&config](){ config.verbose_logging = true; };
    flag_parsers["--use-default-projectm-visualizer"] = [&config](){ config.use_default_projectm_visualizer = true; };
    flag_parsers["--favorites-only-shuffle"] = [&config](){ config.favorites_only_shuffle = true; };

    for (size_t i = 0; i < args.size(); ++i) {
        std::string arg = args[i];
        std::string key = arg;
        std::string value;

        size_t equals_pos = arg.find('=');
        if (equals_pos != std::string::npos) {
            key = arg.substr(0, equals_pos);
            value = arg.substr(equals_pos + 1);
        }

        if (key.rfind("--", 0) == 0) {
            auto it = parsers.find(key);
            if (it != parsers.end()) {
                if (!value.empty()) {
                    it->second(value);
                } else if (i + 1 < args.size()) {
                    it->second(args[++i]);
                }
                continue;
            }

            auto flag_it = flag_parsers.find(key);
            if (flag_it != flag_parsers.end()) {
                if (equals_pos != std::string::npos) {
                     Logger::error("Flag option does not take a value: " + key);
                     display_help(argv[0]);
                     return false;
                }
                flag_it->second();
                continue;
            }

            // If we are here, it's an unknown option
            Logger::error("Unknown option: " + key);
            display_help(argv[0]);
            return false;
        } else {
            // Positional argument
            config.audio_file_paths.push_back(arg);
        }
    }
    return true;
}
