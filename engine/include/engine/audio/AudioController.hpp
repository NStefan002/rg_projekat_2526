/**
 * @file AudioController.hpp
 * @brief Defines the AudioController class that manages the miniaudio engine instance.
 */

#ifndef MATF_RG_PROJECT_AUDIO_CONTROLLER_HPP
#define MATF_RG_PROJECT_AUDIO_CONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <miniaudio.h>

namespace engine::audio {

/**
 * @class AudioController
 * @brief Engine controller responsible for initializing and terminating the miniaudio engine.
 *
 * Initializes a `ma_engine` instance during @ref initialize and shuts it down during
 * @ref terminate. The engine instance is exposed via @ref engine_handle so that
 * @ref engine::resources::ResourcesController can load @ref engine::resources::Audio
 * resources from it during its own initialization phase.
 *
 * If the audio engine fails to initialize, the controller logs a warning and
 * engine_handle() returns nullptr — all audio loading will be skipped gracefully.
 *
 * You do not need to interact with this controller directly to play audio.
 * Use engine::resources::ResourcesController::audio() to obtain engine::resources::Audio instances.
 */
class AudioController final : public core::Controller {
public:
    std::string_view name() const override {
        return "AudioController";
    }

    /**
     * @brief Returns a pointer to the underlying ma_engine, or nullptr if not initialized.
     *
     * Intended for use by engine::resources::ResourcesController to load audio files.
     */
    ma_engine *engine_handle();

    /**
     * @brief Returns true if the miniaudio engine was initialized successfully.
     */
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
