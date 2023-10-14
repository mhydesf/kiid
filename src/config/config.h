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

struct SearchBoxConfig {
    int width;
    int height;
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;

    QString bg_color;
    QString brd_color;
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
            ? QString::fromStdString(json.at("bg_color").get<std::string>()) : "white";
        config.brd_color = json.contains("brd_color")
            ? QString::fromStdString(json.at("brd_color").get<std::string>()) : "gray";
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
        config.bg_color = "white";
        config.brd_color = "gray";
        config.text = " Kiid Search";
        config.cs_string = config.CreateCSString(); 

        return config;
    }

    QString CreateCSString() {
        return QString("QLineEdit {"
                       "background-color: %1;"
                       "border: %2px solid %3;"
                       "border-radius: %4px;"
                       "font-size: %5px;"
                       "padding: %6px;"
                       "}")
            .arg(bg_color)
            .arg(brd_width)
            .arg(brd_color)
            .arg(brd_radius)
            .arg(font_size)
            .arg(padding);
    }
};

struct ResultsViewConfig {
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;
    int num_items;

    QString bg_color;
    QString brd_color;
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
            ? QString::fromStdString(json.at("bg_color").get<std::string>()) : "white";
        config.brd_color = json.contains("brd_color")
            ? QString::fromStdString(json.at("brd_color").get<std::string>()) : "gray";
        if (json.contains("v_scroll_bar")) {
            config.v_scroll_bar = json.at("v_scroll_bar") ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff;
        }
        if (json.contains("h_scroll_bar")) {
            config.h_scroll_bar = json.at("h_scroll_bar") ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff;
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
        config.bg_color = "white";
        config.brd_color = "gray";
        config.cs_string = config.CreateCSString();
        config.v_scroll_bar = Qt::ScrollBarAlwaysOff;
        config.h_scroll_bar = Qt::ScrollBarAlwaysOff;

        return config;
    }

    QString CreateCSString() {
        return QString(
            "QListWidget {"
            "background-color: %1; "
            "border: %2px solid %3; "
            "border-radius: %4px; "
            "font-size: %5px; "
            "padding: %6px;"
            "}")
        .arg(bg_color)
        .arg(brd_width)
        .arg(brd_color)
        .arg(brd_radius)
        .arg(font_size)
        .arg(padding);
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

struct Config {
    SearchBoxConfig sb_config;
    ResultsViewConfig rv_config;
    ScreenConfig screen_config;

    static Config Load(const Json& json = Json{}) {
        Config config;
        config.sb_config = json.contains("search_box") ? SearchBoxConfig::Load(json.at("search_box"))
                                : SearchBoxConfig::Default();
        config.rv_config = json.contains("results_view") ? ResultsViewConfig::Load(json.at("results_view"))
                                : ResultsViewConfig::Default();
        config.screen_config = json.contains("screen") ? ScreenConfig::Load(json.at("screen"))
                                : ScreenConfig::Default();

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

