#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
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
    std::string bg_color;
    std::string brd_color;
    std::string text;

    std::string cs_string;

    static SearchBoxConfig Load(const Json& json) {
        SearchBoxConfig config;
        config.width = json.contains("width") ? json.at("width").get<int>() : 600;
        config.height = json.contains("height") ? json.at("height").get<int>() : 60;
        config.brd_width = json.contains("brd_width") ? json.at("brd_width").get<int>() : 1;
        config.brd_radius = json.contains("brd_radius") ? json.at("brd_radius").get<int>() : 10;
        config.font_size = json.contains("font_size") ? json.at("font_size").get<int>() : 18;
        config.padding = json.contains("padding") ? json.at("padding").get<int>() : 5;
        config.bg_color = json.contains("bg_color") ? json.at("bg_color").get<std::string>() : "white";
        config.brd_color = json.contains("brd_color") ? json.at("brd_color").get<std::string>() : "gray";
        config.text = json.contains("text") ? json.at("text").get<std::string>() : " Kiid Search";
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

    std::string CreateCSString() {
        std::string str = fmt::format("\
            background-color: {};       \
            border: {}px solid {};      \
            border-radius: {}px;        \
            font-size: {};              \
            padding: {};                \
        ",
        bg_color,
        brd_width,
        brd_color,
        brd_radius,
        font_size,
        padding);
        return str;
    }
};

struct ResultsViewConfig {
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;
    int num_items;
    std::string bg_color;
    std::string brd_color;
    std::string cs_string;
    Qt::ScrollBarPolicy v_scroll_bar = Qt::ScrollBarAlwaysOff;
    Qt::ScrollBarPolicy h_scroll_bar = Qt::ScrollBarAlwaysOff;

    static ResultsViewConfig Load(const Json& json) {
        ResultsViewConfig config;
        config.brd_width = json.contains("brd_width") ? json.at("brd_width").get<int>() : 1;
        config.brd_radius = json.contains("brd_radius") ? json.at("brd_radius").get<int>() : 10;
        config.font_size = json.contains("font_size") ? json.at("font_size").get<int>() : 18;
        config.padding = json.contains("padding") ? json.at("padding").get<int>() : 5;
        config.num_items = json.contains("num_items") ? json.at("num_items").get<int>() : 5;
        config.bg_color = json.contains("bg_color") ? json.at("bg_color").get<std::string>() : "white";
        config.brd_color = json.contains("brd_color") ? json.at("brd_color").get<std::string>() : "gray";
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

    std::string CreateCSString() {
        std::string str = fmt::format("\
            background-color: white;   \
            border: 1px solid gray;    \
            border-radius: 10px;       \
            font-size: 18px;           \
            padding: 5px;              \
        ",
        bg_color,
        brd_width,
        brd_color,
        brd_radius,
        font_size,
        padding);
        return str;
    }
};

struct Config {
    SearchBoxConfig sb_config;
    ResultsViewConfig rv_config;

    static Config Load(const Json& json = Json{}) {
        Config config;
        config.sb_config = json.contains("search_box") ? SearchBoxConfig::Load(json.at("search_box"))
                                : SearchBoxConfig::Default();
        config.rv_config = json.contains("results_view") ? ResultsViewConfig::Load(json.at("results_view"))
                                : ResultsViewConfig::Default();

        return config;
    }

};

static inline Config LoadConfigFromFile(std::string& filepath) {
    Json data;
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

