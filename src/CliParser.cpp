// src/CliParser.cpp
#include "CliParser.h"
#include "Config.h"
#include "utils/Logger.h"
#include <vector>
#include <functional>
#include <unordered_map>

void CliParser::display_help(const std::string &program_name) {
  std::cout << "Aurora Visualizer v" << APP_VERSION << "\n\n"
            << "Usage: " << program_name
            << " [options] [audio_file_1 audio_file_2 ...]\n\n"

            << "--- Display & Performance ---\n"
            << "  --width <px>               Set window width (default: 1024).\n"
            << "  --height <px>              Set window height (default: 640).\n"
            << "  --fps <value>              Set frames per second (default: 30).\n\n"

            << "--- Text & Font ---\n"
            << "  --font-path <path>         Path to the font file (TTF/OTF).\n"
            << "  --song-info-font-size <pt> Font size for song title and artist.\n"
            << "  --url-font-size <pt>       Font size for the URL text.\n"
            << "  --song-info-color <hex>    Font color for song info (e.g., FFFFFF).\n"
            << "  --url-color <hex>          Font color for URL (e.g., 00A3B1).\n"
            << "  --song-info-border-color <hex> Color for the song info text border.\n"
            << "  --url-border-color <hex>   Color for the URL text border.\n"
            << "  --song-info-border-thickness <val> Thickness of the song info text border.\n"
            << "  --url-border-thickness <val>   Thickness of the URL text border.\n\n"

            << "--- Text Content & Visibility ---\n"
            << "  --title <text>             Custom text to display as the title.\n"
            << "  --artist-name <name>       Set artist name overlay.\n"
            << "  --url-text <text>          Set URL text overlay.\n"
            << "  --hide-title               Hide the song title text.\n"
            << "  --hide-artist              Hide the artist name text.\n"
            << "  --hide-url                 Hide the URL text.\n"
            << "  --hide-border              Hide the text border.\n\n"

            << "--- Animation ---\n"
            << "  --disable-text-animation   Disable the text animation.\n"
            << "  --transition-time <sec>    Duration of the fade effect at the end.\n"
            << "  --pre-fade-delay <sec>     Time to bounce at full opacity before fading.\n"
            << "  --bounce-duration <sec>    How long the text bounces before fading.\n"
            << "  --bounce-speed <pixels/sec> Speed of the bouncing text.\n"
            << "  --bounce-randomness <val>  Amount of randomness to apply to bounce angle.\n"
            << "  --fade-to-min-duration <sec> Time it takes to fade to minimum transparency.\n"
            << "  --min-transparency <0-1>   The minimum transparency for the text.\n"
            << "  --disable-breathing-effect Disable the text breathing effect.\n"
            << "  --breathing-amount <val>   The amount the text size changes for the breathing effect.\n"
            << "  --breathing-speed <val>    The speed of the text breathing effect.\n\n"

            << "--- Presets ---\n"
            << "  --preset-duration <sec>    Time before switching to the next preset.\n"
            << "  --preset-blend-time <sec>  Time for the blend transition between presets.\n"
            << "  --preset-list-file <path>  Path to a file containing a list of .milk presets.\n"
            << "  --broken-preset-directory <path> Directory to move broken presets to.\n"
            << "  --favorites-file <path>    Path to the favorites file.\n"
            << "  --next-preset-key <key>    Key to load the next random preset (e.g., 'n').\n"
            << "  --prev-preset-key <key>    Key to load the previous preset (e.g., 'p').\n"
            << "  --mark-broken-preset-key <key> Key to mark the current preset as broken (e.g., 'b').\n"
            << "  --favorite-preset-key <key> Key to mark the current preset as a favorite (e.g., 'f').\n"
            << "  --use-default-projectm-visualizer Use projectM's default visualizer for testing audio input.\n\n"

            << "--- Recording ---\n"
            << "  --record-video             Enable video recording.\n"
            << "  --audio-input-mode <mode>  Set audio input mode (SystemDefault, PipeWire, PulseAudio, File). Default: PipeWire.\n"
            << "  --pipewire-sink-name <name> Set the name of the virtual PipeWire sink (default: AuroraSink).\n"
            << "  --output-directory <path>  Directory to save recorded videos.\n"
            << "  --video-framerate <value>  Set video recording framerate.\n"
            << "  --ffmpeg-command <cmd>     The ffmpeg command template for recording.\n\n"

            << "--- Other ---\n"
            << "  --audio-file <path>        Add an audio file to the playlist (can be used multiple times).\n"
            << "  --version                  Display application version.\n"
            << "  --verbose                  Enable verbose logging.\n"
            << "  -h, --help                 Display this help message.\n";
}

bool CliParser::parse(Config& config, int argc, char *argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
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
    parsers["--ffmpeg-command"] = [&config](const std::string& v){ strncpy(config.ffmpeg_command, v.c_str(), sizeof(config.ffmpeg_command) - 1); config.ffmpeg_command[sizeof(config.ffmpeg_command) - 1] = '\0'; };
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


    for (size_t i = 0; i < args.size(); ++i) {
        const std::string &arg = args[i];
        if (arg.rfind("--", 0) == 0) {
            auto it = parsers.find(arg);
            if (it != parsers.end()) {
                if (i + 1 < args.size()) {
                    it->second(args[++i]);
                }
            } else {
                auto flag_it = flag_parsers.find(arg);
                if (flag_it != flag_parsers.end()) {
                    flag_it->second();
                } else {
                    Logger::error("Unknown option: " + arg);
                    display_help(argv[0]);
                    return false;
                }
            }
        } else {
            config.audio_file_paths.push_back(arg);
        }
    }
    return true;
}
