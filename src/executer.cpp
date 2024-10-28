#include <executer.hpp>

#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include <array>
#include <mutex>
#include <iostream>

namespace Engine {
    void Execute(const std::string& command) {
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((command + " 2>&1").c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }

        std::array<char, 128> buffer;
        std::lock_guard<std::mutex> guard(log_mutex);  // Блокируем мьютекс на время работы с логами
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            logs.emplace_back(buffer.data());
        }
    }

    // Функция для получения всех логов
    std::vector<std::string> GetLogs() {
        std::lock_guard<std::mutex> guard(log_mutex);  // Защищаем доступ к логам
        return logs;
    }

    // Функция для очистки логов
    void ClearLogs() {
        std::lock_guard<std::mutex> guard(log_mutex);  // Защищаем доступ к логам
        logs.clear();
    }
}
