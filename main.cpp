#include <window/kiid_window.h>
#include <config/config.h>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Kiid::Window::KiidWindow window;
    window.show();

    return app.exec();
}

