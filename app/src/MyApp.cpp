#include <MyApp.hpp>
#include <MyController.hpp>
#include <MyGUIController.hpp>
#include <engine/core/App.hpp>
#include <spdlog/spdlog.h>

void MyApp::app_setup() {
    spdlog::info("MyApp::app_setup");

    auto main_ctrl = engine::core::App::register_controller<MyController>();
    main_ctrl->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    spdlog::info("Registered controller: {}", main_ctrl->name());

    auto gui_ctrl = engine::core::App::register_controller<MyGUIController>();
    gui_ctrl->after(main_ctrl);

    spdlog::info("Registered controller: {}", gui_ctrl->name());
}
