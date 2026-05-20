#include <engine/audio/AudioController.hpp>
#include <spdlog/spdlog.h>

namespace engine::audio {

void AudioController::initialize() {
    if (ma_engine_init(nullptr, &m_engine) != MA_SUCCESS) {
        spdlog::warn("AudioController: failed to initialize audio engine");
        return;
    }
    m_initialized = true;
    spdlog::info("AudioController: audio engine initialized successfully");
}

// unused overrides

bool AudioController::loop() { return true; }

void AudioController::poll_events() {}

void AudioController::update() {}

void AudioController::begin_draw() {}

void AudioController::draw() {}

void AudioController::end_draw() {}

void AudioController::terminate() {
    if (m_initialized) {
        ma_engine_uninit(&m_engine);
        m_initialized = false;
        spdlog::info("AudioController: audio engine terminated");
    }
}

ma_engine *AudioController::engine_handle() {
    return m_initialized ? &m_engine : nullptr;
}

bool AudioController::is_initialized() const {
    return m_initialized;
}

}// namespace engine::audio
