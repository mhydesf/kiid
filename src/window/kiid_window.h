#include "QtCore/qnamespace.h"
#include <QtGui/QFontDatabase>
#include <QtGui/QKeyEvent>

#include <executor/executor.h>
#include <window/layout.h>
#include <window/search_box.h>
#include <window/results_view.h>
#include <window/kiid_screen.h>

namespace Kiid::Window {

enum class KiidState {
    APP_LAUNCH = 0,
    CMD_LAUNCH = 1,
    FILE_SEARCH = 2
};

class KiidWindow : public QWidget {
    Q_OBJECT

public:
    using Config = Config::Config;
    using Coordinates = KiidScreen::Coordinates;

    KiidWindow(const Config& config)
        : m_config{config}
        , m_executor{config.exec_config} {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setStyleSheet("background:transparent;");
        this->setAttribute(Qt::WA_TranslucentBackground);
        
        m_layout = new KiidLayout(config.layout_config, this);
        m_search_box = new KiidSearchBox(config.sb_config, this);
        m_results_view = new KiidResultsView(config.rv_config, this);
        m_layout->addWidget(m_search_box);
        m_layout->addStretch(1);
        m_layout->addWidget(m_results_view);
        m_screen = new KiidScreen(m_search_box);
        SetupFonts(m_config.font_config);

        this->StateTransition(KiidState::APP_LAUNCH);

        connect(m_search_box, &QLineEdit::textChanged, this, &KiidWindow::HandleSearch);
        
        const int hor = config.screen_config.p_horizontal;
        const int ver = config.screen_config.p_vertical;
        Coordinates coor = m_screen->GetScreenPosition(hor, ver);
        move(coor.hor, coor.ver);
    }

private slots:
    void HandleSearch(const QString& text) {
        switch (m_state) {
        case KiidState::APP_LAUNCH:
            AppSearch(text);
            break;
        case KiidState::CMD_LAUNCH:
            // no searching when launching commands
            break;
        case KiidState::FILE_SEARCH:
            break;
        };
    }

    void keyPressEvent(QKeyEvent* event) {
        using Direction = Kiid::Window::KiidResultsView::SelectionDirection;
        switch (event->key()) {
        case Qt::Key_Escape:
            ProcessSearchBoxExit();
            break;
        case Qt::Key_Return:
            Execute();
            break;
        case Qt::Key_Tab:
            m_results_view->SetActiveItem(Direction::FORWARD, 1, 0);
            break;
        case Qt::Key_Backtab:
            m_results_view->SetActiveItem(Direction::BACKWARD, 1, m_results_view->count() - 1);
            break;
        case Qt::Key_AsciiTilde:
            this->StateTransition(KiidState::APP_LAUNCH);
            break;
        case Qt::Key_Colon:
            this->StateTransition(KiidState::CMD_LAUNCH);
            break;
        case Qt::Key_Question:
            this->StateTransition(KiidState::FILE_SEARCH);
            break;
        case Qt::Key_Control:
            // Control key segfaults when sent to search_box :shrug:
            break;
        case Qt::Key_Shift:
            // Shift key segfaults when sent to search_box :shrug:
            break;
        default:
            QApplication::sendEvent(m_search_box, event);
            break;
        }
    }

private:
    void Execute() {
        switch (m_state) {
        case KiidState::APP_LAUNCH:
        {
            if (m_results_view->currentItem() == nullptr) { break; }
            auto app_name = m_results_view->currentItem()->text();
            auto app = m_executor.GetAppByName(app_name.toStdString());
            (*app).Execute();
            close();
            break;
        }
        case KiidState::CMD_LAUNCH:
        {
            if (m_search_box->text().isEmpty()) { break; }
            auto cmd = m_search_box->text();
            close();
            m_executor.Execute(cmd.toStdString());
            break;
        }
        case KiidState::FILE_SEARCH:
            break;
        }
    }

    void StateTransition(const KiidState& state) {
        m_state = state;
        Kiid::Config::ColorConfig state_color;
        switch (m_state) {
        case KiidState::APP_LAUNCH:
            state_color = m_config.state_config.app_launch;
            break;
        case KiidState::CMD_LAUNCH:
            state_color = m_config.state_config.cmd_launch;
            break;
        case KiidState::FILE_SEARCH:
            state_color = m_config.state_config.file_search;
            break;
        }
        m_search_box->UpdateBorder(state_color.r, state_color.g, state_color.b, state_color.opacity);
        m_results_view->UpdateBorder(state_color.r, state_color.g, state_color.b, state_color.opacity);
    }

    void ProcessSearchBoxExit() {
        if (m_search_box->isDefaultState()) {
            close();
        } else {
            m_results_view->clear();
            m_search_box->clear();
            this->StateTransition(KiidState::APP_LAUNCH);
        }
    }

    void AppSearch(const QString& text) {
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

    void SetupFonts(const Kiid::Config::FontConfig& config) {
        m_search_box->setFont(config.font_name);
        m_results_view->setFont(config.font_name);
    }

private:
    KiidScreen* m_screen;
    KiidLayout* m_layout;
    KiidSearchBox* m_search_box;
    KiidResultsView* m_results_view;

    Kiid::Config::Config m_config;
    Kiid::Executor::Executor m_executor;
    KiidState m_state = KiidState::APP_LAUNCH;
};

} // namespace Kiid::Window

