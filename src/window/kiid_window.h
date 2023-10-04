#include <QtWidgets/QApplication>
#include <QtGui/QFontDatabase>
#include <QtGui/QKeyEvent>
#include <QtGui/QScreen>

#include <executor/executor.h>
#include <window/layout.h>
#include <window/search_box.h>
#include <window/results_view.h>

namespace Kiid::Window {

class KiidWindow : public QWidget {
    Q_OBJECT

public:
    KiidWindow() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setStyleSheet("background:transparent;");
        this->setAttribute(Qt::WA_TranslucentBackground);
        
        QScreen* screen = QGuiApplication::primaryScreen();
        if (screen) {
            move((screen->size().width() - width()) / 2, 400);
        }
        
        m_layout = new KiidLayout(this);
        m_search_box = new KiidSearchBox(this);
        m_results_view = new KiidResultsView(this);
        m_layout->addWidget(m_search_box);
        m_layout->addStretch(1);
        m_layout->addWidget(m_results_view);
        SetupFonts();

        m_executor.LoadApplications();

        connect(m_search_box, &QLineEdit::textChanged, this, &KiidWindow::HandleSearch);
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
        
        results.isEmpty() ? m_results_view->hide() : m_results_view->show();
    }

    void keyPressEvent(QKeyEvent* event) {
        switch (event->key()) {
        case Qt::Key_Escape:
            if (m_results_view->count() > 0) {
                m_results_view->clear();
                m_search_box->clear();
            } else {
                close();
            }
            break;
        case Qt::Key_Return:
            if (m_results_view->currentItem() != nullptr) {
                Execute(m_results_view->currentItem()->text());
            }
            break;
        case Qt::Key_Tab:
        {
            int nextRow = m_results_view->currentRow() + 1;
            if (nextRow >= m_results_view->count()) { nextRow = 0; }
            m_results_view->setCurrentRow(nextRow);
            break;
        }
        case Qt::Key_Backtab:
        {
            int nextRow = m_results_view->currentRow() - 1;
            if (nextRow < 0) { nextRow = m_results_view->count() - 1; }
            m_results_view->setCurrentRow(nextRow);
            break;
        }
        case Qt::Key_Shift:
            // Shift key segfaults when sent to search_box :shrug:
            break;
        default:
            QApplication::sendEvent(m_search_box, event);
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

    void Execute(const QString& app_name) {
        auto app = m_executor.GetAppByName(app_name.toStdString());
        int result = (*app).Execute();
        if (!result) { close(); }
    }

private:
    KiidLayout* m_layout;
    KiidSearchBox* m_search_box;
    KiidResultsView* m_results_view;

    Kiid::Executor::Executor m_executor{};
};

} // namespace Kiid::Window

