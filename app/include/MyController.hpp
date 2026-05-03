#pragma once
#include <engine/core/Engine.hpp>
#include <string_view>

class MyController final : public engine::core::Controller {
private:
    // camera constants (used to initialize the camera position and front vector)
    const glm::vec3 camera_start_position = {13.0f, 1.0f, 0.0f};
    const glm::vec3 camera_start_front = {-1.0f, -0.01f, -0.01f};

    // cursor state
    bool cursor_enabled = false;

    void draw_model();
    void draw_skybox();
    // restricts camera movement to a certain area
    void restrict_camera();

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
