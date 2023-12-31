#pragma once

#include <map>
#include <memory>
#include <string>
#include <functional>
#include <filesystem>
#include <fmt/format.h>

#include "app.h"
#include "config/config.h"

namespace Kiid::Executor {

static const std::string DEFAULT_APP_DIR = "/usr/share/applications/";

class Executor {
public:
    Executor(const Kiid::Config::ExecutorConfig& config)
        : m_config{config}
    {
        LoadApplications();
    }

    Executor(const Executor& other) = delete;
    Executor& operator=(const Executor& other) = delete;

    Executor(const Executor&& other)
        : m_applications{std::move(other.m_applications)} {}

    Executor& operator=(const Executor&& other) {
        if (this == &other) {
            return *this;
        }
        m_applications = std::move(other.m_applications);

        return *this;
    }

public:
    int Execute(const std::string& cmd) {
        if (isStandaloneCommand(cmd)) {
            return ExecuteQuite(cmd);
        }
        std::string terminal_cmd = fmt::format("{} -e bash -c '{}; exec bash'", m_config.terminal, cmd);
        return system(terminal_cmd.c_str());
    }

    int ExecuteQuite(const std::string& cmd) {
        std::string bg_cmd = cmd + " &";
        return system(bg_cmd.c_str());
    }

    void LoadApplications(const std::filesystem::path& path = DEFAULT_APP_DIR) {
        if (!std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            return;
        }
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            Application app = ParseApplication(entry.path().string());
            std::shared_ptr<Application> app_ptr = std::make_shared<Application>(app);
            if ((*app_ptr).Name.empty()) {
                app_ptr.reset();
                continue;
            }
            m_applications.emplace(app.Name, app_ptr);
        }
    }

    std::vector<std::string> GetHits(const std::string& search) {
        std::vector<std::string> vec{};
        for (const auto& entry : m_applications) {
            if (toLower(entry.first).find(toLower(search)) != std::string::npos) {
                vec.push_back(entry.first);
            }
        }
        return vec;
    }

    std::shared_ptr<Application> GetAppByName(const std::string& app_name) {
        return m_applications.at(app_name);
    }

private:
    bool isStandaloneCommand(const std::string& cmd) {
        return m_config.standalones.find(cmd) != m_config.standalones.end();
    }

    static std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return lower;
    }

private:
    Kiid::Config::ExecutorConfig m_config;
    std::map<std::string, std::shared_ptr<Application>> m_applications;
};

} // namespace Kiid::Executor
