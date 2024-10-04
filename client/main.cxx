#include "app.hxx"

auto main(const int argc, char** argv) -> int {
    Application app(argc, argv);
    return Application::exec();
}
