#pragma once
#include <engine/core/Engine.hpp>
#include <string_view>

class MyController final : public engine::core::Controller {
private:
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
