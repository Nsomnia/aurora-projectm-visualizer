// src/utils/common.cpp
#include "utils/common.h"
#include <algorithm>
#include <sstream>

std::string sanitize_filename(const std::string &filepath) {
    std::string filename = filepath.substr(filepath.find_last_of('/') + 1);
    size_t dot_pos = filename.rfind('.');
    if (dot_pos != std::string::npos) {
        filename = filename.substr(0, dot_pos);
    }
    std::replace(filename.begin(), filename.end(), '_', ' ');
    return filename;
}

std::string sanitize_for_filename(const std::string& input) {
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
    std::string invalid_chars = "<>:\x22\x2F\x5C|\x3F\x2A\x27\x28\x29"; // "<>":/\\|?*'()"
    for (char& c : output) {
        if (invalid_chars.find(c) != std::string::npos) {
            c = '_';
        }
    }
    return output;
}

std::vector<std::string> wrapText(const std::string &text,
                                  int lineLengthTarget) {
  std::string processedText = text;
  std::replace(processedText.begin(), processedText.end(), '_', ' ');

  std::stringstream ss(processedText);
  std::string word;
  std::string currentLine;
  std::vector<std::string> lines;

  while (ss >> word) {
    if (static_cast<int>(currentLine.length() + word.length() + 1) >
            lineLengthTarget &&
        !currentLine.empty()) {
      lines.push_back(currentLine);
      currentLine = "";
    }
    if (!currentLine.empty()) {
      currentLine += " ";
    }
    currentLine += word;
  }
  if (!currentLine.empty()) {
    lines.push_back(currentLine);
  }

  return lines;
}
