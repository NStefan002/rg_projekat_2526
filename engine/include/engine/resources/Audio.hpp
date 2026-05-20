#ifndef MATF_RG_PROJECT_AUDIO_HPP
#define MATF_RG_PROJECT_AUDIO_HPP

#include <filesystem>
#include <miniaudio.h>

namespace engine::resources {

class Audio {
    friend class ResourcesController;

public:
    void play();

    void pause();

    void stop();

    bool is_playing() const;

    void set_looping(bool looping);

    void set_volume(float volume);

    void destroy();

    ~Audio() = default;

private:
    explicit Audio(ma_engine *engine, const std::filesystem::path &path);

    ma_sound m_sound{};
    bool m_loaded = false;
};

}// namespace engine::resources

#endif//MATF_RG_PROJECT_AUDIO_HPP
