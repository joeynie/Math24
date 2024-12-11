#include "logger.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level) {
    logLevel = level;
}

void Logger::setLogFile(const std::string& filename) {
    logFile.open(filename, std::ios::out | std::ios::app); //app:不重写，加在文件末尾
}

void Logger::log(const std::string& message,LogLevel level) {
    if (static_cast<int>(level) < static_cast<int>(logLevel)) return; // level低于设定的logLevel，不输出

    // std::lock_guard<std::mutex> lock(mtx);
    std::string levelStr = levelMap.at(level);
    std::string timestamp = currentTimestamp();

    if (logFile.is_open()) {
        logFile << timestamp << " [" << levelStr << "] " << message << std::endl;
    }

    std::cout << timestamp << " [" << levelStr << "] " << message << std::endl;
}

std::string Logger::currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time_t_now);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

template<typename T>
void Logger::log_vector(const std::vector<T>& vec, LogLevel level) {
    if (static_cast<int>(level) < static_cast<int>(logLevel)) return; // level低于设定的logLevel，不输出
    std::string levelStr = levelMap.at(level);
    std::string timestamp = currentTimestamp();
    
    if(logFile.is_open()){
        logFile << timestamp << " [" << levelStr << "] Nums: ";
        for(auto i : vec){
            logFile << i << " ";
        }
        logFile << std::endl;
    }
    std::cout << timestamp << " [" << levelStr << "] Nums: ";
    for(auto i : vec){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
template void Logger::log_vector<float>(const std::vector<float>&, LogLevel);
template void Logger::log_vector<std::string>(const std::vector<std::string>&, LogLevel);