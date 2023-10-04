#include <window/kiid_window.h>
#include <config/config.h>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    std::string CONFIG_PATH = "/home/mhyde/config.json";
    Kiid::Config::Config config = Kiid::Config::LoadConfigFromFile(CONFIG_PATH);

    Kiid::Window::KiidWindow window{config};
    window.show();

    return app.exec();
}

