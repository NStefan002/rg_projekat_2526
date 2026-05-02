#pragma once
#include <engine/core/Engine.hpp>
#include <string_view>

class MyController final : public engine::core::Controller {
private:
    void draw_statue();
    void draw_skybox();

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
