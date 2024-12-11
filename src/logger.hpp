#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
// #include <mutex>
#include <string>
#include <ctime>
#include <unordered_map>
#include <vector>
enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
public:
    static Logger& getInstance() ;

    // 设置日志级别
    void setLogLevel(LogLevel level) ;
    // 设置文件输出
    void setLogFile(const std::string& filename) ;
    // 记录日志
    void log(const std::string& message,LogLevel level=LogLevel::INFO) ;
    
    template<typename T>
    void log_vector(const std::vector<T>& vec, LogLevel level=LogLevel::INFO) ;
    Logger() : logLevel(LogLevel::INFO) {} // 默认日志级别是INFO
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    // 获取当前时间戳
    std::string currentTimestamp();

    std::unordered_map<LogLevel, std::string> levelMap = {
        {LogLevel::INFO, "INFO"},
        {LogLevel::WARNING, "WARNING"},
        {LogLevel::ERROR, "ERROR"},
        {LogLevel::DEBUG, "DEBUG"}
    };

    LogLevel logLevel;
    std::ofstream logFile;
    // std::mutex mtx; 
};

#endif // LOGGER_H