#pragma once

#include <QtWidgets/QLineEdit>
#include <QtCore/qtmetamacros.h>

namespace Kiid::Window {

class KiidSearchBox : public QLineEdit {
    Q_OBJECT
public:
    KiidSearchBox() {}
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
};

} // namespace Kiid::Window

