#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <config/config.h>

namespace Kiid::Window {

class KiidSearchBox : public QLineEdit {
    Q_OBJECT

public:
    using SearchBoxConfig = Config::SearchBoxConfig;

    KiidSearchBox(const SearchBoxConfig& config,
                  QWidget* parent = nullptr)
        : m_config{std::move(config)}
        , QLineEdit(parent)
    {
        Setup(config);
    }
    ~KiidSearchBox() {}

    KiidSearchBox(const KiidSearchBox& other) = delete;
    KiidSearchBox& operator=(const KiidSearchBox& other) = delete;

    KiidSearchBox(const KiidSearchBox&& other) {}
    KiidSearchBox& operator=(const KiidSearchBox&& other) {
        if (this == &other) {
            return *this;
        }

        return *this;
    }

private:
    void Setup(const SearchBoxConfig& config) {
        this->setPlaceholderText(config.text.c_str());
        this->setStyleSheet(config.cs_string.c_str());
        this->setFixedWidth(config.width);
        this->setFixedHeight(config.height);
        this->setFocusPolicy(Qt::NoFocus);
    }

    SearchBoxConfig m_config;
};

} // namespace Kiid::Window

