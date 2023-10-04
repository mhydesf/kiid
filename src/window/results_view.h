#pragma once

#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

namespace Kiid::Window {

class KiidResultsView : public QListWidget {
    Q_OBJECT

public:
    KiidResultsView(QWidget* parent = nullptr)
        : QListWidget(parent)
    {
        Setup();
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
    void Setup() {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setStyleSheet("     \
            background-color: white;        \
            border: 1px solid gray;         \
            border-radius: 10px;            \
            font-size: 18px;                \
            padding: 5px;                   \
        ");
        this->setFocusPolicy(Qt::NoFocus);
        this->setFixedHeight(5*24);
        this->setVisible(false);
    }
};

} // namespace Kiid::Window


