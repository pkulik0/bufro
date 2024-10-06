#include "app.hxx"
#include "version.hxx"

auto main(const int argc, char** argv) -> int {
    qDebug() << "Version:" << VERSION << "Build time:" << BUILD_TIME;

    Application app(argc, argv);
    return Application::exec();
}
