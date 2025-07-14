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