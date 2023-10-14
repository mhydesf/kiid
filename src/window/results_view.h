#pragma once

#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>
#include <config/config.h>

namespace Kiid::Window {

class KiidResultsView : public QListWidget {
    Q_OBJECT

public:
    using ResultsViewConfig = Config::ResultsViewConfig;

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


