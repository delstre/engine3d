#pragma once

#include <vector>
#include <string>
#include <mutex>

namespace Engine {
    static std::vector<std::string> logs;
    static std::mutex log_mutex;

    void Execute(const std::string& command);
    std::vector<std::string> GetLogs();
    void ClearLogs();
}
