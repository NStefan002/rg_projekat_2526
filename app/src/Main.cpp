#include <MyApp.hpp>
#include <engine/core/Engine.hpp>

/**
 * Start here...
 */
int main(int argc, char **argv) {
    return std::make_unique<MyApp>()->run(argc, argv);
}
