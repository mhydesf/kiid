#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtCore/qtmetamacros.h>

namespace Kiid::Window {

class KiidLayout : public QVBoxLayout {
    Q_OBJECT

public:
    KiidLayout(QWidget* parent = nullptr)
        : QVBoxLayout(parent)
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
        this->setContentsMargins(5, 5, 5, 5);
    }
};

} // namespace Kiid::Window

