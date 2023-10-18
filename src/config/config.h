#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <QtCore/QString>
#include <QtCore/qnamespace.h>

namespace Kiid::Config {

using Json = nlohmann::json;

template <typename T>
T LoadFromJsonField(const Json& json, const std::string& key, const T& _default) {
    return json.contains(key) ? json.at(key).get<T>() : _default;
}

struct ColorConfig {
    int r;
    int g;
    int b;
    float opacity;

    static ColorConfig Load(const Json& json) {
        ColorConfig config;
        config.r = LoadFromJsonField(json, "r", 0);
        config.g = LoadFromJsonField(json, "g", 0);
        config.b = LoadFromJsonField(json, "b", 0);
        config.opacity = LoadFromJsonField(json, "opacity", 1.0f);

        return config;
    }

    static ColorConfig Default(const int r, const int g, const int b) {
        ColorConfig config;
        config.r = r;
        config.g = g;
        config.b = b;
        config.opacity = 1.0;

        return config;
    }
};

const ColorConfig DEFAULT_COLOR_WHITE = {255, 255, 255, 1.0f};
const ColorConfig DEFAULT_COLOR_BLACK = {0, 0, 0, 1.0f};

struct SearchBoxConfig {
    int width;
    int height;
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;

    ColorConfig bg_color;
    ColorConfig brd_color;
    ColorConfig text_color;

    QString text;
    QString cs_string;

    static SearchBoxConfig Load(const Json& json) {
        SearchBoxConfig config;
        config.width = LoadFromJsonField(json, "width", 600);
        config.height = LoadFromJsonField(json, "height", 60);
        config.brd_width = LoadFromJsonField(json, "brd_width", 1);
        config.brd_radius = LoadFromJsonField(json, "brd_radius", 10);
        config.font_size = LoadFromJsonField(json, "font_size", 18);
        config.padding = LoadFromJsonField(json, "paddings", 5);
        
        config.bg_color = json.contains("bg_color")
            ? ColorConfig::Load(json.at("bg_color")) : DEFAULT_COLOR_WHITE;
        
        config.brd_color = json.contains("brd_color")
            ? ColorConfig::Load(json.at("brd_color")) : DEFAULT_COLOR_BLACK;
        
        config.text_color = json.contains("text_color")
            ? ColorConfig::Load(json.at("text_color")) : DEFAULT_COLOR_BLACK;

        config.text = json.contains("text")
            ? QString::fromStdString(json.at("text").get<std::string>()) : " Kiid Search";
        
        config.cs_string = config.CreateCSString(); 

        return config;
    }

    static SearchBoxConfig Default() {
        SearchBoxConfig config;
        config.width = 600;
        config.height = 60;
        config.brd_width = 1;
        config.brd_radius = 10;
        config.font_size = 18;
        config.padding = 5;
        config.bg_color = DEFAULT_COLOR_WHITE;
        config.brd_color = DEFAULT_COLOR_BLACK;
        config.text_color = DEFAULT_COLOR_BLACK;
        config.text = " Kiid Search";
        config.cs_string = config.CreateCSString(); 

        return config;
    }

    QString CreateCSString() {
        return QString(
            "QLineEdit {"
                "background-color: rgba(%1, %2, %3, %4);"
                "border: %5px solid rgba(%6, %7, %8, %9);"
                "border-radius: %10px;"
                "padding: %11px;"
                "font-size: %12px;"
                "color: rgb(%13, %14, %15)"
            "}"
        )
        .arg(bg_color.r)
        .arg(bg_color.g)
        .arg(bg_color.b)
        .arg(bg_color.opacity)
        .arg(brd_width)
        .arg(brd_color.r)
        .arg(brd_color.g)
        .arg(brd_color.b)
        .arg(brd_color.opacity)
        .arg(brd_radius)
        .arg(padding)
        .arg(font_size)
        .arg(text_color.r)
        .arg(text_color.g)
        .arg(text_color.b);
    }
};

struct ResultsViewConfig {
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;
    int num_items;

    ColorConfig bg_color;
    ColorConfig brd_color;
    ColorConfig text_color;

    QString cs_string;
    Qt::ScrollBarPolicy v_scroll_bar = Qt::ScrollBarAlwaysOff;
    Qt::ScrollBarPolicy h_scroll_bar = Qt::ScrollBarAlwaysOff;

    static ResultsViewConfig Load(const Json& json) {
        ResultsViewConfig config;
        config.brd_width = LoadFromJsonField(json, "brd_width", 1);
        config.brd_radius = LoadFromJsonField(json, "brd_radius", 10);
        config.font_size = LoadFromJsonField(json, "font_size", 18);
        config.padding = LoadFromJsonField(json, "padding", 5);
        config.num_items = LoadFromJsonField(json, "num_items", 5);
        
        config.bg_color = json.contains("bg_color")
            ? ColorConfig::Load(json.at("bg_color")) : DEFAULT_COLOR_WHITE;
        
        config.brd_color = json.contains("brd_color")
            ? ColorConfig::Load(json.at("brd_color")) : DEFAULT_COLOR_BLACK;
        
        config.text_color = json.contains("text_color")
            ? ColorConfig::Load(json.at("text_color")) : DEFAULT_COLOR_BLACK;
        
        if (json.contains("v_scroll_bar")) {
            config.v_scroll_bar = json.at("v_scroll_bar")
                ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff;
        }
        
        if (json.contains("h_scroll_bar")) {
            config.h_scroll_bar = json.at("h_scroll_bar")
                ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff;
        }
        
        config.cs_string = config.CreateCSString();

        return config;
    }

    static ResultsViewConfig Default() {
        ResultsViewConfig config;
        config.brd_width = 1;
        config.brd_radius = 10;
        config.font_size = 18;
        config.padding = 5;
        config.num_items = 5;
        config.bg_color = DEFAULT_COLOR_WHITE;
        config.brd_color = DEFAULT_COLOR_BLACK;
        config.text_color = DEFAULT_COLOR_BLACK;
        config.cs_string = config.CreateCSString();
        config.v_scroll_bar = Qt::ScrollBarAlwaysOff;
        config.h_scroll_bar = Qt::ScrollBarAlwaysOff;

        return config;
    }

    QString CreateCSString() {
        return QString(
            "QListWidget {"
                "background-color: rgba(%1, %2, %3, %4);"
                "border: %5px solid rgba(%6, %7, %8, %9);"
                "border-radius: %10px;"
                "padding: %11px;"
                "font-size: %12px;"
            "}"
            "QListWidget::item {"
                "color: rgb(%13, %14, %15)"
            "}"
        )
        .arg(bg_color.r)
        .arg(bg_color.g)
        .arg(bg_color.b)
        .arg(bg_color.opacity)
        .arg(brd_width)
        .arg(brd_color.r)
        .arg(brd_color.g)
        .arg(brd_color.b)
        .arg(brd_color.opacity)
        .arg(brd_radius)
        .arg(padding)
        .arg(font_size)
        .arg(text_color.r)
        .arg(text_color.g)
        .arg(text_color.b);
    };
};

struct ScreenConfig {
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
    int p_horizontal;
    int p_vertical;

    static ScreenConfig Load(const Json& json) {
        ScreenConfig config;
        if (json.contains("p_horizontal")) {
            if (json.at("p_horizontal").is_number_integer()) {
                config.p_horizontal = json.at("p_horizontal").get<int>();
            } else {
                if (json.at("p_horizontal") == "middle") { config.p_horizontal = -1; }
                if (json.at("p_horizontal") == "left") { config.p_horizontal = -2; }
                if (json.at("p_horizontal") == "right") { config.p_horizontal = -3; }
            }
        } else {
            config.p_horizontal = -1;
        }
        if (json.contains("p_vertical")) {
            if (json.at("p_vertical").is_number_integer()) {
                config.p_vertical = json.at("p_vertical").get<int>();
            } else {
                if (json.at("p_vertical") == "middle") { config.p_vertical = -1; }
                if (json.at("p_vertical") == "top") { config.p_vertical = -2; }
                if (json.at("p_vertical") == "bottom") { config.p_vertical = -3; }
            }
        } else {
            config.p_vertical = -1;
        }
        config.p_horizontal = json.contains("p_horizontal") ? json.at("p_horizontal").get<int>() : 1;
        config.p_vertical = json.contains("p_vertical") ? json.at("p_vertical").get<int>() : 10;

        return config;
    }

    static ScreenConfig Default() {
        ScreenConfig config;
        config.p_horizontal = -1;
        config.p_vertical = -1;

        return config;
    }
};

struct FontConfig {
    QString font_name;

    static FontConfig Load(const Json& json) {
        FontConfig config;
        config.font_name = json.contains("font_name")
            ? QString::fromStdString(json.at("font_name").get<std::string>()) : "Monospace";
        return config;
    }

    static FontConfig Default() {
        FontConfig config;
        config.font_name = "Monospace";
        return config;
    }
};
struct Config {
    SearchBoxConfig sb_config;
    ResultsViewConfig rv_config;
    ScreenConfig screen_config;
    FontConfig font_config;

    static Config Load(const Json& json = Json{}) {
        Config config;
        config.sb_config = json.contains("search_box") ? SearchBoxConfig::Load(json.at("search_box"))
                                : SearchBoxConfig::Default();
        config.rv_config = json.contains("results_view") ? ResultsViewConfig::Load(json.at("results_view"))
                                : ResultsViewConfig::Default();
        config.screen_config = json.contains("screen") ? ScreenConfig::Load(json.at("screen"))
                                : ScreenConfig::Default();
        config.font_config = json.contains("font") ? FontConfig::Load(json.at("font"))
                                : FontConfig::Default();

        return config;
    }

};

static inline std::string LoadConfigPath() {
    std::string defaultConfigPath = std::string(std::getenv("HOME")) + "/.config";
    const char* xdgConfigHome = std::getenv("XDG_CONFIG_HOME");

    std::string configDir = xdgConfigHome ? xdgConfigHome : defaultConfigPath;
    std::filesystem::path filepath = configDir + "/kiid/config.json";
    return filepath.string();
}

static inline Config LoadConfigFromFile() {
    Json data;
    std::string filepath = LoadConfigPath();
    std::ifstream input_file(filepath, std::ifstream::binary);

    if (input_file.is_open()) {
        try {
            input_file >> data;
        } catch (Json::parse_error& e) {
            std::cerr << "Error parsing JSON: " << e.what() << '\n';
        }
    }

    return Config::Load(data);
}

} // namespace Kiid::Config

