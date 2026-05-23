#include "engine/util/Errors.hpp"
#include <engine/resources/Audio.hpp>
#include <spdlog/spdlog.h>

namespace engine::resources {

Audio::Audio(ma_engine *engine, const std::filesystem::path &path) {
    if (ma_sound_init_from_file(engine, path.string().c_str(),
                                MA_SOUND_FLAG_DECODE, nullptr, nullptr,
                                &m_sound) == MA_SUCCESS) {
        m_loaded = true;
        spdlog::info("[ResourcesController]: initialized audio '{}'", path.string());
    } else {
        throw util::EngineError(util::EngineError::Type::AssetLoadingError,
                                std::format("Failed to initialize audio '{}'", path.string()));
    }
}

void Audio::play() {
    if (m_loaded) {
        ma_sound_start(&m_sound);
    }
}

void Audio::pause() {
    if (m_loaded) {
        ma_sound_stop(&m_sound);
    }
}

void Audio::stop() {
    if (m_loaded) {
        ma_sound_stop(&m_sound);
        ma_sound_seek_to_pcm_frame(&m_sound, 0);
    }
}

bool Audio::is_playing() const {
    return m_loaded && ma_sound_is_playing(&m_sound);
}

void Audio::set_looping(bool looping) {
    if (m_loaded) {
        ma_sound_set_looping(&m_sound, looping ? MA_TRUE : MA_FALSE);
    }
}

void Audio::set_volume(float volume) {
    if (m_loaded) {
        ma_sound_set_volume(&m_sound, volume);
    }
}

void Audio::destroy() {
    if (m_loaded) {
        ma_sound_uninit(&m_sound);
        m_loaded = false;
    }
}

}// namespace engine::resources
