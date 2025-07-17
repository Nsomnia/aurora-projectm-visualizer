// src/audio_input.cpp
#include "audio_input.h"
#include "utils/Logger.h"
#include <vector>
#include <taglib/tag.h>
#include <taglib/fileref.h>

AudioInput::AudioInput(Config& config) : _config(config), _music(nullptr) {
    _audio_data.pM = nullptr;
}

AudioInput::~AudioInput() {
    cleanup();
}

bool AudioInput::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    Mix_SetPostMix(audio_callback, &_audio_data);
    return true;
}

void AudioInput::load_and_play_music(const std::string& music_file) {
    if (_music) {
        Mix_FreeMusic(_music);
        _music = nullptr;
    }

    TagLib::FileRef f(music_file.c_str());
    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        _config.songTitle = tag->title().toCString(true);
        _config.artistName = tag->artist().toCString(true);
    }

    if (_config.songTitle.empty()) {
        _config.songTitle = sanitize_filename(music_file);
    }
    if (_config.artistName.empty()) {
        _config.artistName = "Unknown Artist";
    }


    _music = Mix_LoadMUS(music_file.c_str());
    if (!_music) {
        Logger::error("Failed to load music: " + music_file + " - " + std::string(Mix_GetError()));
        return;
    }
    Mix_PlayMusic(_music, 1);
}

void AudioInput::cleanup() {
    if (_music) {
        Mix_FreeMusic(_music);
        _music = nullptr;
    }
    Mix_CloseAudio();
}

void AudioInput::set_projectm_handle(projectm_handle pM) {
    _audio_data.pM = pM;
}

double AudioInput::get_audio_duration() const {
    if (_music) {
        return Mix_MusicDuration(_music);
    }
    return 0.0;
}

void AudioInput::audio_callback(void* userdata, Uint8* stream, int len) {
    AudioData* audioData = static_cast<AudioData*>(userdata);
    if (!audioData || !audioData->pM) {
        return;
    }

    int samples = len / (2 * sizeof(int16_t));
    std::vector<float> float_samples(len / sizeof(int16_t));
    int16_t* pcm_stream = reinterpret_cast<int16_t*>(stream);

    for (int i = 0; i < len / 2; ++i) {
        float_samples[i] = static_cast<float>(pcm_stream[i]) / 32768.0f;
    }
    projectm_pcm_add_float(audioData->pM, float_samples.data(), samples, PROJECTM_STEREO);
}
