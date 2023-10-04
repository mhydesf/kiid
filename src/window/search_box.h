#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtCore/qtmetamacros.h>

namespace Kiid::Window {

class KiidSearchBox : public QLineEdit {
    Q_OBJECT

public:
    KiidSearchBox(QWidget* parent = nullptr)
        : QLineEdit(parent)
    {
        Setup();
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
    void Setup() {
        this->setPlaceholderText(" Kiid Search");
        this->setStyleSheet("       \
            background-color: white;        \
            border: 1px solid gray;         \
            border-radius: 10px;            \
            font-size: 18px;                \
            padding: 5px;                   \
        ");
        this->setFixedWidth(600);
        this->setFixedHeight(60);
    }
};

} // namespace Kiid::Window

