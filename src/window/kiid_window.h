#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtCore/QStringListModel>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>

namespace Kiid::Window {

class KiidWindow : public QWidget {
public:
    KiidWindow() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setStyleSheet("background:transparent;");
        this->setAttribute(Qt::WA_TranslucentBackground);
        
        SetupScreen();
        SetupLayout();
        SetupSearchBox();
        SetupResultsList();
        SetupFonts();

        connect(m_search_box, &QLineEdit::textChanged, this, &KiidWindow::HandleSearch);

        AdjustWindowSize(QStringList());
    }

private slots:
    void HandleSearch(const QString& text) {
        QStringList results;

        if (text.contains("example", Qt::CaseInsensitive)) {
            results << "Example Result 1" << "Example Result 2"<< "Example Result 3"<< "Example Result 4"<< "Example Result 5";
        }

        m_results_view->clear();
        m_results_view->addItems(results);

        results.isEmpty() ? m_results_view->hide() : m_results_view->show();
        AdjustWindowSize(results);
    }

private:
    void SetupFonts() {
        int fontId = QFontDatabase::addApplicationFont("/home/mhyde/.local/share/fonts/FiraMonoNerdFontMono-Medium.otf");
        if (fontId != -1) {
            QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
            QFont font(family);
            m_search_box->setFont(font);
            m_results_view->setFont(font);
        }
    }

    void SetupScreen() {
        m_screen = QGuiApplication::primaryScreen();
        if (m_screen) {
            move((m_screen->size().width() - width()) / 2, 400);
        }
    }

    void SetupLayout() {
        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(5, 5, 5, 5);
    }

    void SetupSearchBox() {
        m_search_box = new QLineEdit(this);
        m_search_box->setPlaceholderText("Kiid Search");
        m_search_box->setStyleSheet("       \
            background-color: white;        \
            border: 1px solid gray;         \
            border-radius: 10px;            \
            font-size: 18px;                \
            padding: 5px;                   \
        ");
        m_search_box->setFixedWidth(600);
        m_search_box->setFixedHeight(60);
        m_layout->addWidget(m_search_box);
    }

    void SetupResultsList() {
        m_results_view = new QListWidget(this);
        m_results_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_results_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_results_view->setStyleSheet("       \
            background-color: white;        \
            border: 1px solid gray;         \
            border-radius: 10px;            \
            font-size: 18px;                \
            padding: 5px;                   \
        ");
        m_layout->addWidget(m_results_view);
        m_results_view->setVisible(false);
    }
    
    void AdjustWindowSize(const QStringList& results) {
        if (results.isEmpty()) {
            m_results_view->setVisible(false);
            setFixedSize(m_search_box->width() + 10, m_search_box->height() + 10);
        } else {
            m_results_view->setVisible(true);
            setFixedSize(m_search_box->width() + 10, m_search_box->height() + m_results_view->height() + 40);
        }
    }

private:
    QScreen* m_screen;
    QVBoxLayout* m_layout;
    QLineEdit* m_search_box;
    QListWidget* m_results_view;
};

} // namespace Kiid::Window

