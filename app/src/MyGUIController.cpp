#include <MyController.hpp>
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
    if (platform->key(engine::platform::KEY_F1).state() == engine::platform::Key::State::JustPressed) {
        show_camera_info = !show_camera_info;
    }
    // toggle help
    if (platform->key(engine::platform::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
        show_help = !show_help;
    }
    // toggle point light menu
    if (platform->key(engine::platform::KEY_F3).state() == engine::platform::Key::State::JustPressed) {
        show_point_light_menu = !show_point_light_menu;
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
    if (show_point_light_menu) {
        draw_point_light_menu();
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
    ImGui::Text("Hold Shift: Increase movement/rotate speed");
    ImGui::Text("Mouse movement: Rotate camera");
    ImGui::Text("Mouse scroll: Zoom camera");
    ImGui::Text("F1: Toggle camera info");
    ImGui::Text("F2: Toggle help");
    ImGui::Text("F3: Toggle point light menu");
    ImGui::Text("F4: Toggle cursor");
    ImGui::Text("F5: Trigger event");
    ImGui::Text("ESC: Exit application");
    ImGui::Text("\nStand to honor the past,\nmove to hear its echo.");
    ImGui::End();
}

void MyGUIController::draw_point_light_menu() {
    auto main_ctrl = engine::core::Controller::get<MyController>();
    ImGui::Begin("Point light menu");
    ImGui::Checkbox("Enable", &main_ctrl->pt_light_enabled);
    ImGui::SliderFloat("Red", &main_ctrl->pt_light_color[0], 0.0f, 1.0f);
    ImGui::SliderFloat("Green", &main_ctrl->pt_light_color[1], 0.0f, 1.0f);
    ImGui::SliderFloat("Blue", &main_ctrl->pt_light_color[2], 0.0f, 1.0f);
    ImGui::SliderFloat("Intensity", &main_ctrl->pt_light_intensity, 0.0f, 10.0f);
    if (ImGui::Button("Reset to default")) {
        main_ctrl->pt_light_color[0] = main_ctrl->pt_light_color_orig[0];
        main_ctrl->pt_light_color[1] = main_ctrl->pt_light_color_orig[1];
        main_ctrl->pt_light_color[2] = main_ctrl->pt_light_color_orig[2];
        main_ctrl->pt_light_intensity = main_ctrl->pt_light_intensity_orig;
    }
    ImGui::End();
}

bool MyGUIController::is_active() const {
    return show_camera_info || show_help || show_point_light_menu;
}
