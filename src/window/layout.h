#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtCore/qtmetamacros.h>

#include <config/config.h>

namespace Kiid::Window {

class KiidLayout : public QVBoxLayout {
    Q_OBJECT

public:
    KiidLayout(Kiid::Config::LayoutConfig config, QWidget* parent = nullptr)
        : m_config{config}
        , QVBoxLayout(parent)
    {
        Setup();
    }
    ~KiidLayout() {}

    KiidLayout(const KiidLayout& other) = delete;
    KiidLayout& operator=(const KiidLayout& other) = delete;

    KiidLayout(const KiidLayout&& other) {}
    KiidLayout& operator=(const KiidLayout&& other) {
        if (this == &other) {
            return *this;
        }

        return *this;
    }

private:
    void Setup() {
        this->setContentsMargins(
            m_config.left,
            m_config.top,
            m_config.right,
            m_config.bottom
        );
    }

private:
    Kiid::Config::LayoutConfig m_config;
};

} // namespace Kiid::Window

