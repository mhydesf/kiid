#pragma once

#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Kiid::Executor {

struct Application {
    std::string Name;
    std::string Exec;
    std::string Icon;
    std::string Type;
    std::string Path;

    static Application ParseApplication(const std::string& filepath) {
        Application app;
        std::ifstream file(filepath);
        std::string line;

        bool inside_block = false;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, value;

            if (line == "[Desktop Entry]") {
                inside_block = true;
                continue;
            }
            
            if (line.size() > 1 && line[0] == '[') {
                break;
            }
            
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                if (key == "Name") app.Name = value;
                else if (key == "Exec") app.Exec = value;
                else if (key == "Icon") app.Icon = value;
                else if (key == "Type") app.Type = value;
                else if (key == "Path") app.Path = value;
            }
        }

        return app;
    }

    int Execute() { return system(Exec.c_str()); }
};

} // namespace Kiid::Executor

