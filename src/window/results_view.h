#pragma once

#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>
#include <config/config.h>

namespace Kiid::Window {

class KiidResultsView : public QListWidget {
    Q_OBJECT

public:
    using ResultsViewConfig = Config::ResultsViewConfig;
    enum class SelectionDirection { FORWARD = 0, BACKWARD };

    KiidResultsView(const ResultsViewConfig& config,
                    QWidget* parent = nullptr)
        : m_config{std::move(config)}
        , QListWidget(parent)
    {
        Setup(m_config);
    }

    ~KiidResultsView() {}

    KiidResultsView(const KiidResultsView& other) = delete;
    KiidResultsView& operator=(const KiidResultsView& other) = delete;

    KiidResultsView(const KiidResultsView&& other) {}
    KiidResultsView& operator=(const KiidResultsView&& other) {
        if (this == &other) {
            return *this;
        }

        return *this;
    }

    void SetActiveItem(SelectionDirection dir, const int& inc, const int& fallback) {
        int nextRow;
        switch (dir) {
        case SelectionDirection::FORWARD:
        {
            nextRow = this->currentRow() + inc;
            if (nextRow >= this->count()) { nextRow = fallback; }
            this->setCurrentRow(nextRow);
            break;
        }
        case SelectionDirection::BACKWARD:
        {
            nextRow = this->currentRow() - inc;
            if (nextRow < 0) { nextRow = fallback; }
            this->setCurrentRow(nextRow);
            break;
        }
        }
    }

    void UpdateBorderColor(const int& r,
                           const int& g,
                           const int& b,
                           const float& opacity) {
        QString currentStyleSheet = this->styleSheet();
        QString newBorder = QString(
            "border: %1px solid rgba(%2, %3, %4, %5);"
        )
        .arg(m_config.brd_width)
        .arg(r)
        .arg(g)
        .arg(b)
        .arg(opacity);
        QString updatedStyleSheet = currentStyleSheet.replace(QRegularExpression("border: [^;]*;"), newBorder);
        this->setStyleSheet(updatedStyleSheet);
    }

private:
    void Setup(const ResultsViewConfig& config) {
        this->setVerticalScrollBarPolicy(config.v_scroll_bar);
        this->setHorizontalScrollBarPolicy(config.h_scroll_bar);
        this->setStyleSheet(config.cs_string);
        this->setFocusPolicy(Qt::NoFocus);
        this->setFixedHeight(config.num_items * (config.font_size + config.padding));
        this->setVisible(false);
    }

    ResultsViewConfig m_config;
};

} // namespace Kiid::Window


