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

struct ColorConfig {
    int r;
    int g;
    int b;
    float opacity;

    static ColorConfig Load(const Json& json) {
        ColorConfig config;
        config.r = json.contains("r") ? json.at("r").get<int>() : 0;
        config.g = json.contains("g") ? json.at("g").get<int>() : 0;
        config.b = json.contains("b") ? json.at("b").get<int>() : 0;
        config.opacity = json.contains("opacity") ? json.at("opacity").get<float>() : 1.0;

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

struct SearchBoxConfig {
    int width;
    int height;
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;

    ColorConfig bg_color;
    ColorConfig brd_color;

    QString text;
    QString cs_string;

    static SearchBoxConfig Load(const Json& json) {
        SearchBoxConfig config;
        config.width = json.contains("width") ? json.at("width").get<int>() : 600;
        config.height = json.contains("height") ? json.at("height").get<int>() : 60;
        config.brd_width = json.contains("brd_width") ? json.at("brd_width").get<int>() : 1;
        config.brd_radius = json.contains("brd_radius") ? json.at("brd_radius").get<int>() : 10;
        config.font_size = json.contains("font_size") ? json.at("font_size").get<int>() : 18;
        config.padding = json.contains("padding") ? json.at("padding").get<int>() : 5;
        
        config.bg_color = json.contains("bg_color")
            ? ColorConfig::Load(json.at("bg_color")) : ColorConfig::Default(255,255,255);
        
        config.brd_color = json.contains("brd_color")
            ? ColorConfig::Load(json.at("brd_color")) : ColorConfig::Default(0,0,0);
        
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
        config.bg_color = ColorConfig::Default(255,255,255);
        config.brd_color = ColorConfig::Default(0,0,0);
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
        .arg(font_size);
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

    QString cs_string;
    Qt::ScrollBarPolicy v_scroll_bar = Qt::ScrollBarAlwaysOff;
    Qt::ScrollBarPolicy h_scroll_bar = Qt::ScrollBarAlwaysOff;

    static ResultsViewConfig Load(const Json& json) {
        ResultsViewConfig config;
        config.brd_width = json.contains("brd_width") ? json.at("brd_width").get<int>() : 1;
        config.brd_radius = json.contains("brd_radius") ? json.at("brd_radius").get<int>() : 10;
        config.font_size = json.contains("font_size") ? json.at("font_size").get<int>() : 18;
        config.padding = json.contains("padding") ? json.at("padding").get<int>() : 5;
        config.num_items = json.contains("num_items") ? json.at("num_items").get<int>() : 5;
        
        config.bg_color = json.contains("bg_color")
            ? ColorConfig::Load(json.at("bg_color")) : ColorConfig::Default(255,255,255);
        
        config.brd_color = json.contains("brd_color")
            ? ColorConfig::Load(json.at("brd_color")) : ColorConfig::Default(0,0,0);
        
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
        config.bg_color = ColorConfig::Default(255,255,255);
        config.brd_color = ColorConfig::Default(0,0,0);
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
        .arg(font_size);
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

