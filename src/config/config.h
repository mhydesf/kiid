#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <nlohmann/json.hpp>

#include <QtCore/QString>
#include <QtCore/qnamespace.h>

namespace Kiid::Config {

using Json = nlohmann::json;

template <typename T>
concept Configuration = requires {
    { T::Load(std::declval<const Json&>()) } -> std::same_as<T>;
    { T::Default() } -> std::same_as<T>;
};

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

    static ColorConfig Default() {
        ColorConfig config;
        config.r = 0;
        config.g = 0;
        config.b = 0;
        config.opacity = 1.0;

        return config;
    }
};
static_assert(Configuration<ColorConfig>);

const int DEFAULT_WIDTH = 600;
const int DEFAULT_HEIGHT = 60;
const int DEFAULT_PADDING = 5;
const int DEFAULT_NUM_ITEMS = 5;
const int DEFAULT_FONT_SIZE = 18;
const int DEFAULT_BORDER_WIDTH = 1;
const int DEFAULT_BORDER_RADIUS = 10;
const QString DEFAULT_TEXT = " Kiid Search";
const QString DEFAULT_FONT = "Monospace";
const ColorConfig DEFAULT_COLOR_BLACK = ColorConfig::Default();
const ColorConfig DEFAULT_COLOR_WHITE = {255, 255, 255, 1.0f};
const ColorConfig DEFAULT_COLOR_RED = {255, 0, 0, 1.0f};
const ColorConfig DEFAULT_COLOR_GREEN = {0, 255, 0, 1.0f};
const ColorConfig DEFAULT_COLOR_BLUE = {0, 0, 255, 1.0f};

struct SearchBoxConfig {
    int width;
    int height;
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;

    ColorConfig bg_color;
    ColorConfig text_color;

    QString text;
    QString cs_string;

    static SearchBoxConfig Load(const Json& json) {
        SearchBoxConfig config;
        config.width = LoadFromJsonField(json, "width", DEFAULT_WIDTH);
        config.height = LoadFromJsonField(json, "height", DEFAULT_HEIGHT);
        config.brd_width = LoadFromJsonField(json, "brd_width", DEFAULT_BORDER_WIDTH);
        config.brd_radius = LoadFromJsonField(json, "brd_radius", DEFAULT_BORDER_RADIUS);
        config.font_size = LoadFromJsonField(json, "font_size", DEFAULT_FONT_SIZE);
        config.padding = LoadFromJsonField(json, "paddings", DEFAULT_PADDING);
        
        config.bg_color = json.contains("bg_color")
            ? ColorConfig::Load(json.at("bg_color")) : DEFAULT_COLOR_WHITE;
        
        config.text_color = json.contains("text_color")
            ? ColorConfig::Load(json.at("text_color")) : DEFAULT_COLOR_BLACK;

        config.text = json.contains("text")
            ? QString::fromStdString(json.at("text").get<std::string>()) : DEFAULT_TEXT;
        
        config.cs_string = config.CreateCSString(); 

        return config;
    }

    static SearchBoxConfig Default() {
        SearchBoxConfig config;
        config.width = DEFAULT_WIDTH;
        config.height = DEFAULT_HEIGHT;
        config.brd_width = DEFAULT_BORDER_WIDTH;
        config.brd_radius = DEFAULT_BORDER_RADIUS;
        config.font_size = DEFAULT_FONT_SIZE;
        config.padding = DEFAULT_PADDING;
        config.bg_color = DEFAULT_COLOR_WHITE;
        config.text_color = DEFAULT_COLOR_BLACK;
        config.text = DEFAULT_TEXT;
        config.cs_string = config.CreateCSString(); 

        return config;
    }

    QString CreateCSString() {
        return QString(
            "QLineEdit {"
                "background-color: rgba(%1, %2, %3, %4);"
                "border: %5px solid rgba(0, 0, 0, 0.0);"
                "border-radius: %6px;"
                "padding: %7px;"
                "font-size: %8px;"
                "color: rgb(%9, %10, %11)"
            "}"
        )
        .arg(bg_color.r)
        .arg(bg_color.g)
        .arg(bg_color.b)
        .arg(bg_color.opacity)
        .arg(brd_width)
        .arg(brd_radius)
        .arg(padding)
        .arg(font_size)
        .arg(text_color.r)
        .arg(text_color.g)
        .arg(text_color.b);
    }
};
static_assert(Configuration<SearchBoxConfig>);

struct ResultsViewConfig {
    int brd_width;
    int brd_radius;
    int font_size;
    int padding;
    int num_items;

    ColorConfig bg_color;
    ColorConfig text_color;

    QString cs_string;
    Qt::ScrollBarPolicy v_scroll_bar = Qt::ScrollBarAlwaysOff;
    Qt::ScrollBarPolicy h_scroll_bar = Qt::ScrollBarAlwaysOff;

    static ResultsViewConfig Load(const Json& json) {
        ResultsViewConfig config;
        config.brd_width = LoadFromJsonField(json, "brd_width", DEFAULT_BORDER_WIDTH);
        config.brd_radius = LoadFromJsonField(json, "brd_radius", DEFAULT_BORDER_RADIUS);
        config.font_size = LoadFromJsonField(json, "font_size", DEFAULT_FONT_SIZE);
        config.padding = LoadFromJsonField(json, "padding", DEFAULT_PADDING);
        config.num_items = LoadFromJsonField(json, "num_items", DEFAULT_NUM_ITEMS);
        
        config.bg_color = json.contains("bg_color")
            ? ColorConfig::Load(json.at("bg_color")) : DEFAULT_COLOR_WHITE;
        
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
        config.brd_width = DEFAULT_BORDER_WIDTH;
        config.brd_radius = DEFAULT_BORDER_RADIUS;
        config.font_size = DEFAULT_FONT_SIZE;
        config.padding = DEFAULT_PADDING;
        config.num_items = DEFAULT_NUM_ITEMS;
        config.bg_color = DEFAULT_COLOR_WHITE;
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
                "border: %5px solid rgba(0, 0, 0, 0.0);"
                "border-radius: %6px;"
                "padding: %7px;"
                "font-size: %8px;"
            "}"
            "QListWidget::item {"
                "color: rgb(%9, %10, %11)"
            "}"
        )
        .arg(bg_color.r)
        .arg(bg_color.g)
        .arg(bg_color.b)
        .arg(bg_color.opacity)
        .arg(brd_width)
        .arg(brd_radius)
        .arg(padding)
        .arg(font_size)
        .arg(text_color.r)
        .arg(text_color.g)
        .arg(text_color.b);
    };
};
static_assert(Configuration<ResultsViewConfig>);

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

        return config;
    }

    static ScreenConfig Default() {
        ScreenConfig config;
        config.p_horizontal = -1;
        config.p_vertical = -1;

        return config;
    }
};
static_assert(Configuration<ScreenConfig>);

struct FontConfig {
    QString font_name;

    static FontConfig Load(const Json& json) {
        FontConfig config;
        config.font_name = json.contains("font_name")
            ? QString::fromStdString(json.at("font_name").get<std::string>()) : DEFAULT_FONT;
        return config;
    }

    static FontConfig Default() {
        FontConfig config;
        config.font_name = DEFAULT_FONT;
        return config;
    }
};
static_assert(Configuration<FontConfig>);

struct StateColorConfig {
    ColorConfig app_launch;
    ColorConfig cmd_launch;
    ColorConfig file_search;

    static StateColorConfig Load(const Json& json = Json{}) {
        StateColorConfig config;
        config.app_launch = json.contains("app_launch")
            ? ColorConfig::Load(json.at("app_launch")) : DEFAULT_COLOR_RED;
        config.cmd_launch = json.contains("cmd_launch")
            ? ColorConfig::Load(json.at("cmd_launch")) : DEFAULT_COLOR_GREEN;
        config.file_search = json.contains("file_search")
            ? ColorConfig::Load(json.at("file_search")) : DEFAULT_COLOR_BLUE;
        return config;
    }

    static StateColorConfig Default() {
        StateColorConfig config;
        config.app_launch = DEFAULT_COLOR_RED;
        config.cmd_launch = DEFAULT_COLOR_GREEN;
        config.file_search = DEFAULT_COLOR_BLUE;
        return config;
    }
};
static_assert(Configuration<StateColorConfig>);

struct ExecutorConfig {
    std::string terminal;
    std::set<std::string> standalones;

    static ExecutorConfig Load(const Json& json) {
        ExecutorConfig config;
        config.terminal = LoadFromJsonField(json, "terminal", std::string{"gnome-terminal"});
        config.standalones = LoadFromJsonField(json, "standalones", std::set<std::string>{});

        return config;
    }

    static ExecutorConfig Default() {
        ExecutorConfig config;
        config.terminal = "gnome-terminal";
        config.standalones = {};

        return config;
    }
};
static_assert(Configuration<ExecutorConfig>);

struct LayoutConfig {
    int left;
    int top;
    int right;
    int bottom;

    static LayoutConfig Load(const Json& json) {
        LayoutConfig config;
        config.left = LoadFromJsonField(json, "left", 5);
        config.top = LoadFromJsonField(json, "top", 5);
        config.right = LoadFromJsonField(json, "right", 5);
        config.bottom = LoadFromJsonField(json, "bottom", 5);

        return config;
    }

    static LayoutConfig Default() {
        LayoutConfig config;
        config.left = 5;
        config.top = 5;
        config.right = 5;
        config.bottom = 5;

        return config;
    }
};
static_assert(Configuration<LayoutConfig>);

struct Config {
    FontConfig font_config;
    ScreenConfig screen_config;
    SearchBoxConfig sb_config;
    ResultsViewConfig rv_config;
    StateColorConfig state_config;
    ExecutorConfig exec_config;
    LayoutConfig layout_config;

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
        config.state_config = json.contains("state_config") ? StateColorConfig::Load(json.at("state_config"))
                                : StateColorConfig::Default();
        config.exec_config = json.contains("executor") ? ExecutorConfig::Load(json.at("executor"))
                                : ExecutorConfig::Default();
        config.layout_config = json.contains("layout") ? LayoutConfig::Load(json.at("layout"))
                                : LayoutConfig::Default();

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

