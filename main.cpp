#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtCore/QStringListModel>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>

#include <nlohmann/json.hpp>

class SpotlightWindow : public QWidget {
public:
    SpotlightWindow() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setFixedSize(400, 200);

        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            move((screen->size().width() - width()) / 2, 100);
        }

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(5, 5, 5, 5);

        QLineEdit* searchBox = new QLineEdit(this);
        searchBox->setPlaceholderText("Search");
        searchBox->setStyleSheet("font-size: 18px; padding: 5px;");
        layout->addWidget(searchBox);

        QListView* resultsView = new QListView(this);
        layout->addWidget(resultsView);

        QStringListModel* model = new QStringListModel(this);
        resultsView->setModel(model);
        resultsView->setStyleSheet("background-color: white; border: none;");

        connect(searchBox, &QLineEdit::textChanged, [=](const QString& text) {
            QStringList results;
            if (text.contains("example", Qt::CaseInsensitive)) {
                results << "Example Result 1" << "Example Result 2";
            }
            model->setStringList(results);
        });

        int fontId = QFontDatabase::addApplicationFont("/home/mhyde/.local/share/fonts/FiraMonoNerdFontMono-Bold.otf");
        if (fontId != -1) {
            QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
            QFont font(family);
            searchBox->setFont(font);
        }

        this->setStyleSheet("background-color: white; border: 1px solid gray; border-radius: 10px;");
    }
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    SpotlightWindow window;
    window.show();

    return app.exec();
}

