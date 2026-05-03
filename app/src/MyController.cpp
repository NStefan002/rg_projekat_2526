#include <MyController.hpp>
#include <MyGUIController.hpp>

std::string_view MyController::name() const {
    return "MyController";
}

void MyController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->Position = camera_start_position;
    camera->Front = camera_start_front;

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->set_enable_cursor(cursor_enabled);
}

bool MyController::loop() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
        return false;
    }
    return true;
}

void MyController::poll_events() {
}

void MyController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float delta_time = platform->dt();

    if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, delta_time);
    }
    if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, delta_time);
    }
    if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, delta_time);
    }
    if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, delta_time);
    }

    // rotate camera with arrow keys
    if (platform->key(engine::platform::KEY_UP).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(0.0f, 5.0f);
    }
    if (platform->key(engine::platform::KEY_DOWN).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(0.0f, -5.0f);
    }
    if (platform->key(engine::platform::KEY_LEFT).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(-5.0f, 0.0f);
    }
    if (platform->key(engine::platform::KEY_RIGHT).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(5.0f, 0.0f);
    }

    if (platform->key(engine::platform::KEY_F3).state() == engine::platform::Key::State::JustPressed) {
        cursor_enabled = !cursor_enabled;
        platform->set_enable_cursor(cursor_enabled);
    }

    if (!cursor_enabled) {
        // use mouse to rotate camera only when cursor is disabled
        // (when the cursor is enabled, the user might want to interact
        // with the GUI instead of rotating the camera)
        auto mouse = platform->mouse();
        camera->rotate_camera(mouse.dx, mouse.dy);
        camera->zoom(mouse.scroll);
    }

    restrict_camera();
}

void MyController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MyController::draw() {
    draw_model();
    draw_skybox();
}

void MyController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void MyController::terminate() {
}

void MyController::draw_model() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    auto kadinjaca_model = engine::core::Controller::get<engine::resources::ResourcesController>()->model("kadinjaca");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

    kadinjaca_model->draw(shader);
}

void MyController::draw_skybox() {
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
    auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox("skybox");
    engine::core::Controller::get<engine::graphics::GraphicsController>()->draw_skybox(shader, skybox_cube);
}

void MyController::restrict_camera() {
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->Position.x = std::clamp(camera->Position.x, -85.0f, 10.0f);
    camera->Position.y = std::clamp(camera->Position.y, 0.0f, 30.0f);
    camera->Position.z = std::clamp(camera->Position.z, -14.0f, 30.0f);
}
