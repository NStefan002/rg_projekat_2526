#include <MyGUIController.hpp>
#include <imgui.h>

std::string_view MyGUIController::name() const {
    return "MyGUIController";
}

void MyGUIController::initialize() {
}

bool MyGUIController::loop() {
    return true;
}

void MyGUIController::poll_events() {
}

void MyGUIController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    // toggle camera info
    if (platform->key(engine::platform::KEY_C).state() == engine::platform::Key::State::JustPressed) {
        show_camera_info = !show_camera_info;
    }
    // toggle help
    if (platform->key(engine::platform::KEY_H).state() == engine::platform::Key::State::JustPressed) {
        show_help = !show_help;
    }
}

void MyGUIController::begin_draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();
}

void MyGUIController::draw() {
    if (show_camera_info) {
        draw_camera_info();
    }
    if (show_help) {
        draw_help();
    }
}

void MyGUIController::end_draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->end_gui();
}

void MyGUIController::terminate() {
}

void MyGUIController::draw_camera_info() {
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

    ImGui::Begin("Camera info");
    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);
    ImGui::Text("(Yaw, Pitch): (%f, %f)", camera->Yaw, camera->Pitch);
    ImGui::Text("Camera front: (%f, %f, %f)", camera->Front.x, camera->Front.y, camera->Front.z);
    ImGui::End();
}

void MyGUIController::draw_help() {
    ImGui::Begin("Help");
    ImGui::Text("WASD: Move camera");
    ImGui::Text("Arrow keys: Rotate camera");
    ImGui::Text("Mouse movement: Rotate camera");
    ImGui::Text("Mouse scroll: Zoom camera");
    ImGui::Text("C: Toggle camera info");
    ImGui::Text("H: Toggle help");
    ImGui::Text("F3: Toggle cursor");
    ImGui::Text("F4: Trigger event");
    ImGui::Text("ESC: Exit application");
    ImGui::End();
}

bool MyGUIController::is_active() const {
    return show_camera_info || show_help;
}
