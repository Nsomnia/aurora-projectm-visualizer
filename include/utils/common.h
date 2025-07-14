// include/visualizer/utils/common.h
#ifndef VISUALIZER_UTILS_COMMON_H
#define VISUALIZER_UTILS_COMMON_H

#include <string>
#include <vector>

// Common utility functions and definitions
std::string sanitize_filename(const std::string &filepath);
std::vector<std::string> wrapText(const std::string &text, int lineLengthTarget);

#endif // VISUALIZER_UTILS_COMMON_H


