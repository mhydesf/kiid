#pragma once

#include <map>
#include <memory>
#include <string>
#include <functional>
#include <filesystem>

#include "app.h"

namespace Kiid::Executor {

static const std::string DEFAULT_APP_DIR = "/usr/share/applications/";

class Executor {
public:
    Executor() { LoadApplications(); }

    Executor(const Executor& other) = delete;
    Executor& operator=(const Executor& other) = delete;

    Executor(const Executor&& other) {}
    Executor& operator=(const Executor&& other) {
        if (this == &other) {
            return *this;
        }

        return *this;
    }

    void LoadApplications(const std::filesystem::path& path = DEFAULT_APP_DIR) {
        if (!std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            return;
        }
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            Application app = Application::ParseApplication(entry.path().string());
            std::shared_ptr<Application> app_ptr = std::make_shared<Application>(app);
            m_applications.emplace(app.Name, app_ptr);
        }
    }

    std::vector<std::string> GetHits(const std::string& search) {
        std::vector<std::string> vec{};
        for (const auto& entry : m_applications) {
            if (entry.first.find(search) != std::string::npos) {
                vec.push_back(entry.first);
            }
        }
        return vec;
    }

    std::shared_ptr<Application> GetAppByName(const std::string& app_name) {
        return m_applications.at(app_name);
    }

    static int Execute(const std::string& cmd) { return system(cmd.c_str()); }

private:
    std::map<std::string, std::shared_ptr<Application>> m_applications;
};

} // namespace Kiid::Executor
