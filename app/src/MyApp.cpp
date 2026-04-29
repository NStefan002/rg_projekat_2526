#include <MyApp.hpp>
#include <MyController.hpp>
#include <engine/core/App.hpp>
#include <spdlog/spdlog.h>

void MyApp::app_setup() {
    spdlog::info("MyApp::app_setup");
    auto my_ctrl = engine::core::App::register_controller<MyController>();
    my_ctrl->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
}
