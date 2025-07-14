#pragma once

#include "Config.h"
#include <SDL_mixer.h>
#include <projectM-4/projectM.h>

struct AudioData {
    projectm_handle pM;
};

class AudioInput {
public:
    AudioInput(Config& config);
    ~AudioInput();

    bool init();
    void load_and_play_music(const std::string& music_file);
    void cleanup();
    Mix_Music* get_music() const { return _music; }

    void set_projectm_handle(projectm_handle pM);

    static void audio_callback(void* userdata, Uint8* stream, int len);

private:
    Config& _config;
    AudioData _audio_data;
    Mix_Music* _music;
};
