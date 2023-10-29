#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtCore/QRegularExpression>

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

    bool isDefaultState() { return this->text().isEmpty(); }

private:
    void Setup(const SearchBoxConfig& config) {
        this->setPlaceholderText(config.text);
        this->setStyleSheet(config.cs_string);
        this->setFixedWidth(config.width);
        this->setFixedHeight(config.height);
        this->setFocusPolicy(Qt::NoFocus);
    }

private:
    SearchBoxConfig m_config;
};

} // namespace Kiid::Window

