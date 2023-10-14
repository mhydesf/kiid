#include <window/kiid_window.h>
#include <config/config.h>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Kiid::Config::Config config = Kiid::Config::LoadConfigFromFile();
    Kiid::Window::KiidWindow window{config};
    window.show();

    return app.exec();
}

