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
        
        config.cs_string = fmt::format("\
            background-color: {};       \
            border: {}px solid {};      \
            border-radius: {}px;        \
            font-size: {};              \
            padding: {};                \
        ",
        config.bg_color,
        config.brd_width,
        config.brd_color,
        config.brd_radius,
        config.font_size,
        config.padding);

        return config;
    }
};

struct ResultsViewConfig {


    static ResultsViewConfig Load(const Json& json) {
        ResultsViewConfig config;

        return config;
    }
};

struct Config {
    SearchBoxConfig sb_config;
    ResultsViewConfig rv_config;

    static Config Load(const Json& json = Json{}) {
        Config config;
        config.sb_config = SearchBoxConfig::Load(json.at("search_box"));
        config.rv_config = ResultsViewConfig::Load(json.at("results_view"));

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
    } else {
        std::cerr << "Could not open the file for reading." << '\n';
    }

    return Config::Load(data);
}

} // namespace Kiid::Config

