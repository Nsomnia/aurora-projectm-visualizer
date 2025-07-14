#pragma once

#include <string>
#include <cstdio>
#include "Config.h"

class VideoExporter {
public:
    VideoExporter(const Config& config);
    ~VideoExporter();

    bool start_export(int width, int height);
    void write_frame(const unsigned char* pixels);
    void end_export();

private:
    const Config& _config;
    FILE* _ffmpeg_pipe;
    int _width;
    int _height;
};
