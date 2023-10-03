#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace Kiid::Config {

using Json = nlohmann::json;

class Config {
public:
    int width;
    int height;

    static Config Load(const Json& json) {
        Config config;
        config.width = json.at("width");
        config.height = json.at("height");

        return config;
    }

};

class ScreenConfig {
public:
    int x_pos;
    int y_pos;

    static ScreenConfig Load(const Json& json) {
        ScreenConfig config;
        config.x_pos = json.at("x_pos");
        config.y_pos = json.at("y_pos");

        return config;
    }
};

static Config LoadConfigFromFile(std::string& filepath) {
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

