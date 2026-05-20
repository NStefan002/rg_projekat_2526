#pragma once
#include <MyGUIController.hpp>
#include <engine/core/Engine.hpp>
#include <vector>

class MyController final : public engine::core::Controller {
private:
    // camera constants (used to initialize the camera position and front vector)
    const glm::vec3 camera_start_position = {13.0f, 1.0f, 0.0f};
    const glm::vec3 camera_start_front = {-1.0f, -0.01f, -0.01f};

    // cursor state
    bool cursor_enabled = false;

    // directional light
    const glm::vec3 dir_light_diffuse_orig = {0.6f, 0.6f, 0.6f};
    const glm::vec3 dir_light_direction_orig = {0.0f, -1.0f, -0.3f};
    glm::vec3 dir_light_direction = dir_light_direction_orig;
    glm::vec3 dir_light_ambient = {0.2f, 0.2f, 0.2f};
    glm::vec3 dir_light_diffuse = dir_light_diffuse_orig;
    glm::vec3 dir_light_specular = {0.4f, 0.4f, 0.4f};

    // point light
    const std::vector<glm::vec3> pt_light_positions = {
            {2.5f, 3.0f, 0.0f},
            {-10.0f, 3.0f, 0.0f},
            {-28.0f, -1.0f, 0.5f},
            {-40.0f, 0.5f, 1.0f},
            {-45.0f, 0.5f, 1.5f},
            {-55.0f, -1.0f, 9.0f},
            {-62.5f, -0.5f, 14.0f},
            {-68.0f, -1.0f, 34.0f},
    };
    const glm::vec3 pt_light_color_orig = {1.0f, 0.7f, 0.3f};
    const float pt_light_intensity_orig = 1.0f;
    glm::vec3 pt_light_color = {1.0f, 0.7f, 0.3f};
    float pt_light_intensity = 1.0f;
    bool pt_light_enabled = false;

    // chain of events
    enum class Stage {
        Day,
        Night,
    };
    Stage event_stage = Stage::Day;
    float event_timer = 0.0f;
    bool event_triggered = false;
    const float night_duration = 10.0f;
    const float day_duration = 10.0f;

    glm::vec3 last_camera_position = camera_start_position;

    void draw_model(engine::resources::Shader *shader);
    void draw_skybox();
    // restricts camera movement to a certain area
    void restrict_camera();
    // updates the light properties in the shader
    void set_light_uniforms(engine::resources::Shader *shader);
    // updates the event stage based on the timer
    void update_event_stage(float delta_time);
    // plays/pauses the audio based on whether the camera is moving
    void play_pause(float delta_time);

    // so that MyGUIController can access the point light properties to display them
    // in the GUI, and to allow MyGUIController to modify the point light properties
    // when the user interacts with the GUI
    friend MyGUIController;

public:
    std::string_view name() const override;
    void initialize() override;
    bool loop() override;
    void poll_events() override;
    void update() override;
    void begin_draw() override;
    void draw() override;
    void end_draw() override;
    void terminate() override;
};
