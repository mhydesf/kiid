#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include <QtGui/QKeyEvent>

#include <QtCore/qnamespace.h>
#include <window/search_box.h>
#include <executor/executor.h>

namespace Kiid::Window {

class KiidWindow : public QWidget {
    Q_OBJECT

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

        m_executor.LoadApplications();

        connect(m_search_box, &QLineEdit::textChanged, this, &KiidWindow::HandleSearch);

        AdjustWindowSize(QStringList());
    }

private slots:
    void HandleSearch(const QString& text) {
        QStringList results;

        if (!text.isEmpty()) {
            std::vector<std::string> resp = m_executor.GetHits(text.toStdString());
            for (const auto& r : resp) {
                results.push_back(QString::fromStdString(r));
            }
        }
        
        if (!results.empty()) {
            m_results_view->clear();
            m_results_view->addItems(results);
        }

        AdjustWindowSize(results);
        results.isEmpty() ? m_results_view->hide() : m_results_view->show();
    }

    void keyPressEvent(QKeyEvent* event) {
        int nextRow = m_results_view->currentRow();
        switch (event->key()) {
        case Qt::Key_Escape:
            if (m_results_view->count() > 0) {
                m_results_view->clear();
                m_search_box->clear();
            } else {
                close();
            }
            break;
        case Qt::Key_Tab:
            nextRow += 1;
            nextRow = nextRow ? nextRow >= m_results_view->count() : 0;
            m_results_view->setCurrentRow(nextRow);
            break;
        case Qt::Key_Backtab:
            nextRow -= 1;
            nextRow = nextRow ? nextRow >= m_results_view->count() : 0;
            m_results_view->setCurrentRow(nextRow);
            break;
        case Qt::Key_Return:
            if (m_results_view->currentItem() != nullptr) { Execute(m_results_view->currentItem()->text()); }
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
        }
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
        m_search_box->setPlaceholderText(" Kiid Search");
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
        m_results_view->setStyleSheet("     \
            background-color: white;        \
            border: 1px solid gray;         \
            border-radius: 10px;            \
            font-size: 18px;                \
            padding: 5px;                   \
        ");
        m_results_view->setFixedHeight(5*24);
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
    
    void Execute(const QString& app_name) {
        auto app = m_executor.GetAppByName(app_name.toStdString());
        (*app).Execute();
    }

private:
    QScreen* m_screen;
    QVBoxLayout* m_layout;
    QLineEdit* m_search_box;
    QListWidget* m_results_view;

    Kiid::Executor::Executor m_executor{};
};

} // namespace Kiid::Window

