#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#include <QtGui/QFontDatabase>
#include <QtGui/QKeyEvent>

#include <window/search_box.h>
#include <window/results_view.h>
#include <window/layout.h>
#include <executor/executor.h>

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
        m_layout->addWidget(m_results_view);
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
    KiidLayout* m_layout;
    KiidSearchBox* m_search_box;
    KiidResultsView* m_results_view;

    Kiid::Executor::Executor m_executor{};
};

} // namespace Kiid::Window

