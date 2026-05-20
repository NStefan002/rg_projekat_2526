/**
 * @file Audio.hpp
 * @brief Defines the Audio resource class.
 */

#ifndef MATF_RG_PROJECT_AUDIO_HPP
#define MATF_RG_PROJECT_AUDIO_HPP

#include <filesystem>
#include <miniaudio.h>

namespace engine::resources {

/**
 * @class Audio
 * @brief Represents a single audio resource loaded from a file.
 *
 * Wraps a `ma_sound` handle from miniaudio. Instances are owned and managed
 * by @ref ResourcesController and should not be deleted by the user.
 *
 * Obtain an instance via @ref ResourcesController::audio().
 *
 * Example:
 * @code
 * auto *resources = engine::core::Controller::get<engine::resources::ResourcesController>();
 * engine::resources::Audio *sound = resources->audio("song");
 * if (sound) {
 *     sound->set_looping(true);
 *     sound->play();
 * }
 * @endcode
 */
class Audio {
    friend class ResourcesController;

public:
    /**
     * @brief Starts or resumes playback.
     */
    void play();

    /**
     * @brief Pauses playback without rewinding.
     */
    void pause();

    /**
     * @brief Stops playback and rewinds to the beginning.
     */
    void stop();

    /**
     * @brief Returns true if the audio is currently playing.
     */
    bool is_playing() const;

    /**
     * @brief Enables or disables looping.
     * @param looping true to loop, false to play once.
     */
    void set_looping(bool looping);

    /**
     * @brief Sets the playback volume.
     * @param volume Value in [0.0, 1.0], where 1.0 is full volume.
     */
    void set_volume(float volume);

    /**
     * @brief Releases the underlying ma_sound handle.
     */
    void destroy();

    ~Audio() = default;

private:
    explicit Audio(ma_engine *engine, const std::filesystem::path &path);

    ma_sound m_sound{};
    bool m_loaded = false;
};

}// namespace engine::resources

#endif//MATF_RG_PROJECT_AUDIO_HPP
