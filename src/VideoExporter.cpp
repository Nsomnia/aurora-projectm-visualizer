#include "VideoExporter.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

// Helper function to sanitize a filename
static std::string sanitize_filename(const std::string& input) {
    std::string output = input;
    // Remove path and extension
    size_t last_slash = output.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        output = output.substr(last_slash + 1);
    }
    size_t last_dot = output.find_last_of('.');
    if (last_dot != std::string::npos) {
        output = output.substr(0, last_dot);
    }
    // Replace invalid characters with underscores
    std::string invalid_chars = "<>:\"/\\|?*";
    for (char& c : output) {
        if (invalid_chars.find(c) != std::string::npos) {
            c = '_';
        }
    }
    return output;
}

// Helper function to replace placeholders in a string
static std::string replace_placeholders(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

VideoExporter::VideoExporter(const Config& config) : _config(config), _ffmpeg_pipe(nullptr) {}

VideoExporter::~VideoExporter() {
    if (_ffmpeg_pipe) {
        end_export();
    }
}

bool VideoExporter::start_export(int width, int height) {
    _width = width;
    _height = height;

    if (strlen(_config.ffmpeg_command) == 0) {
        std::cerr << "Error: ffmpeg_command is not set in the configuration." << std::endl;
        return false;
    }

    std::string sanitized_filename = "output";
    if (!_config.audio_file_paths.empty()) {
        sanitized_filename = sanitize_filename(_config.audio_file_paths[0]);
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string timestamp = ss.str();

    std::string output_path = _config.video_directory + "/" + sanitized_filename + "_" + timestamp + ".mp4";

    std::string command = _config.ffmpeg_command;
    command = replace_placeholders(command, "{WIDTH}", std::to_string(_width));
    command = replace_placeholders(command, "{HEIGHT}", std::to_string(_height));
    command = replace_placeholders(command, "{FPS}", std::to_string(_config.video_framerate));
    if (!_config.audio_file_paths.empty()) {
        command = replace_placeholders(command, "{AUDIO_FILE_PATH}", "\"" + _config.audio_file_paths[0] + "\"");
    } else {
        command = replace_placeholders(command, "-i \"{AUDIO_FILE_PATH}\"", "");
    }
    command = replace_placeholders(command, "{OUTPUT_PATH}", "\"" + output_path + "\"");

    std::cout << "Starting ffmpeg with command: " << command << std::endl;

    _ffmpeg_pipe = popen(command.c_str(), "w");
    if (!_ffmpeg_pipe) {
        std::cerr << "Error: Could not open ffmpeg pipe." << std::endl;
        return false;
    }

    return true;
}

void VideoExporter::end_export() {
    if (_ffmpeg_pipe) {
        pclose(_ffmpeg_pipe);
        _ffmpeg_pipe = nullptr;
        std::cout << "Stopped ffmpeg process." << std::endl;
    }
}

void VideoExporter::write_frame(const unsigned char* pixels) {
    if (_ffmpeg_pipe) {
        fwrite(pixels, 1, _width * _height * 3, _ffmpeg_pipe);
    }
}