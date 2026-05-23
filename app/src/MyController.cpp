#include <MyController.hpp>
#include <spdlog/spdlog.h>

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
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_F5).state() == engine::platform::Key::State::JustPressed) {
        event_triggered = true;
        spdlog::info("Event triggered");
    }
}

void MyController::update() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float delta_time = platform->dt();

    bool shift_pressed = platform->key(engine::platform::KEY_LEFT_SHIFT).state() == engine::platform::Key::State::Pressed ||
                         platform->key(engine::platform::KEY_RIGHT_SHIFT).state() == engine::platform::Key::State::Pressed;

    if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, delta_time * (shift_pressed ? 3.0f : 1.0f));// move faster when shift is held
    }
    if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, delta_time * (shift_pressed ? 3.0f : 1.0f));
    }
    if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, delta_time * (shift_pressed ? 3.0f : 1.0f));
    }
    if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, delta_time * (shift_pressed ? 3.0f : 1.0f));
    }

    // rotate camera with arrow keys
    if (platform->key(engine::platform::KEY_UP).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(0.0f, shift_pressed ? 15.0f : 5.0f);// rotate faster when shift is held
    }
    if (platform->key(engine::platform::KEY_DOWN).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(0.0f, shift_pressed ? -15.0f : -5.0f);
    }
    if (platform->key(engine::platform::KEY_LEFT).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(shift_pressed ? -15.0f : -5.0f, 0.0f);
    }
    if (platform->key(engine::platform::KEY_RIGHT).state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(shift_pressed ? 15.0f : 5.0f, 0.0f);
    }

    if (platform->key(engine::platform::KEY_F4).state() == engine::platform::Key::State::JustPressed) {
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

    if (event_triggered) {
        update_event_stage(delta_time);
    }
    play_pause(delta_time);
    restrict_camera();
}

void MyController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MyController::draw() {
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("lighting");
    set_light_uniforms(shader);
    draw_model(shader);
    draw_skybox();
}

void MyController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void MyController::terminate() {
}

void MyController::draw_model(engine::resources::Shader *shader) {
    auto kadinjaca_model = engine::core::Controller::get<engine::resources::ResourcesController>()->model("kadinjaca");
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

void MyController::set_light_uniforms(engine::resources::Shader *shader) {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_mat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
    shader->set_vec3("view_pos", graphics->camera()->Position);

    // directional light
    shader->set_vec3("dir_light.direction", dir_light_direction);
    shader->set_vec3("dir_light.ambient", dir_light_ambient);
    shader->set_vec3("dir_light.diffuse", dir_light_diffuse);
    shader->set_vec3("dir_light.specular", dir_light_specular);

    // point light
    glm::vec3 pt_light_diffuse = pt_light_color * pt_light_intensity;
    glm::vec3 pt_light_ambient = pt_light_diffuse * 0.1f;
    glm::vec3 pt_light_specular = pt_light_color;
    shader->set_int("num_point_lights", static_cast<int>(pt_light_positions.size()));
    for (size_t i = 0; i < pt_light_positions.size(); i++) {
        shader->set_vec3(std::format("point_light_positions[{}]", i), pt_light_positions[i]);
    }
    shader->set_vec3("point_light.ambient", pt_light_ambient);
    shader->set_vec3("point_light.diffuse", pt_light_diffuse);
    shader->set_vec3("point_light.specular", pt_light_specular);
    shader->set_float("point_light.constant", 1.0f);
    shader->set_float("point_light.linear", 0.09f);
    shader->set_float("point_light.quadratic", 0.032f);
    shader->set_bool("point_light.enabled", pt_light_enabled);
}

void MyController::update_event_stage(float delta_time) {
    event_timer += delta_time;
    switch (event_stage) {
        case Stage::Day: {
            if (event_timer >= night_duration) {
                event_stage = Stage::Night;
                event_timer = 0.0f;
                event_triggered = false;
                spdlog::info("Event stage changed to Night");
                break;
            }
            if (event_timer >= night_duration / 2 && !pt_light_enabled) {
                // at the halfway point of the day stage, enable the point light to simulate turning on the street lights
                pt_light_enabled = true;
                spdlog::info("Point light enabled");
            }
            // gradually decrease the directional light intensity to simulate the sun setting
            float progress = std::min(1.0f, event_timer / night_duration);
            dir_light_diffuse = (1.0f - progress) * dir_light_diffuse_orig;

            // noon (90 deg) -> sunset (0 deg)
            float dir_light_angle = glm::radians(90.0f * (1.0f - progress));
            dir_light_direction = glm::vec3(-glm::cos(dir_light_angle), -glm::sin(dir_light_angle), -0.3f);
            break;
        }
        case Stage::Night: {
            if (event_timer >= day_duration) {
                event_stage = Stage::Day;
                event_timer = 0.0f;
                event_triggered = false;
                dir_light_diffuse = dir_light_diffuse_orig;
                dir_light_direction = dir_light_direction_orig;
                spdlog::info("Event stage changed to Day");
                break;
            }
            if (event_timer >= day_duration / 2 && pt_light_enabled) {
                // at the halfway point of the night stage, disable the point light to simulate turning off the street lights
                pt_light_enabled = false;
                spdlog::info("Point light disabled");
            }
            // gradually increase the directional light intensity to simulate the sun rising
            float progress = std::min(1.0f, event_timer / day_duration);
            dir_light_diffuse = progress * dir_light_diffuse_orig;

            // sunrise (0 deg) -> noon (90 deg)
            float dir_light_angle = glm::radians(90.0f * progress);
            dir_light_direction = glm::vec3(glm::cos(dir_light_angle), -glm::sin(dir_light_angle), -0.3f);
            break;
        }
    }
}

void MyController::play_pause(float delta_time) {
    auto *resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto *sound = resources->audio("kadinjaca");
    if (!sound) {
        return;
    }

    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    bool moved = glm::distance(camera->Position, last_camera_position) > 0.01f;
    last_camera_position = camera->Position;
    if (!moved) {
        if (sound->is_playing()) {
            sound->pause();
            spdlog::info("Audio paused");
        }
    } else {
        if (!sound->is_playing()) {
            sound->play();
            spdlog::info("Audio started/resumed");
        }
    }
}
