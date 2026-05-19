#ifndef MATF_RG_PROJECT_AUDIO_CONTROLLER_HPP
#define MATF_RG_PROJECT_AUDIO_CONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <miniaudio.h>

namespace engine::audio {

class AudioController final : public core::Controller {
public:
    std::string_view name() const override {
        return "AudioController";
    }

    ma_engine *engine_handle();

    bool is_initialized() const;

private:
    void initialize() override;
    bool loop() override;
    void poll_events() override;
    void update() override;
    void begin_draw() override;
    void draw() override;
    void end_draw() override;
    void terminate() override;

    ma_engine m_engine{};
    bool m_initialized = false;
};

}// namespace engine::audio

#endif//MATF_RG_PROJECT_AUDIO_CONTROLLER_HPP
