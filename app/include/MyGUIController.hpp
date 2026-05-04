#pragma once
#include <engine/core/Engine.hpp>
#include <string_view>

class MyGUIController final : public engine::core::Controller {
private:
    bool show_camera_info = false;
    bool show_help = true;
    bool show_point_light_menu = false;

    void draw_camera_info();
    void draw_help();
    void draw_point_light_menu();

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

    bool is_active() const;
};
