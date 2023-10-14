#pragma once

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>

#include <config/config.h>
#include <window/search_box.h>

namespace Kiid::Window {

class KiidScreen {
public:
    using Config = Config::Config;

    struct Coordinates {
        int hor;
        int ver;
    };

    KiidScreen(KiidSearchBox* search_box)
    {
        m_search_box = search_box;
    }
    ~KiidScreen() {}

    Coordinates GetScreenPosition(const int& hor, const int& ver) {
        /**
         *  Position is in pixels relative to the top right corner.
         *          
         *             -> +h
         *           | --------------------
         *           v |
         *          +v |
         *             |        SCREEN
         *             |
         *
         *  Special codes to execute functions
         *      -1 = middle
         *      -2 = h -> left  | v -> top
         *      -3 = h -> right | v -> bottom
         */
        Coordinates coor;
        switch (hor) {
        case -1:
            coor.hor = (m_screen->size().width() - m_search_box->width()) / 2;
            break;
        case -2:
            coor.hor = 50;
            break;
        case -3:
            coor.hor = m_screen->size().width() - ((m_search_box->width() / 2)) - 50;
            break;
        default:
            coor.hor = hor;
            break;
        }
        
        switch (ver) {
        case -1:
            coor.ver = (m_screen->size().height() - m_search_box->height()) / 2;
            break;
        case -2:
            coor.ver = 50;
            break;
        case -3:
            coor.ver = m_screen->size().height() - ((m_search_box->height() / 2)) - 50;
            break;
        default:
            coor.ver = ver;
            break;
        }

        return coor;
    }

private:
    QScreen* m_screen = QGuiApplication::primaryScreen();
    KiidSearchBox* m_search_box;
};

} // namespace Kiid::Window
