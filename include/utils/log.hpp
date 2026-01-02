#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#define SD_EMERG   "<0>"
#define SD_ALERT   "<1>"
#define SD_CRIT    "<2>"
#define SD_ERR     "<3>"
#define SD_WARNING "<4>"
#define SD_NOTICE  "<5>"
#define SD_INFO    "<6>"
#define SD_DEBUG   "<7>"

namespace utils {

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

namespace logtargets {

    class ILOG {
    public:
        virtual ~ILOG() = default;
        virtual void log(LogLevel level, const std::string& message) = 0;
    };

    class ConsoleLog : public ILOG {
    public:
        void log(LogLevel level, const std::string& message) override {
            std::cout << message << std::endl;
        }
    };

    class SystemdLog : public ILOG {
    public:
        void log(LogLevel level, const std::string& message) override {
            switch (level) {
                case utils::LogLevel::ERROR:    std::cout << SD_ERR; break;
                case utils::LogLevel::WARN:     std::cout << SD_WARNING; break;
                case utils::LogLevel::INFO:     std::cout << SD_INFO; break;
                case utils::LogLevel::DEBUG:    std::cout << SD_DEBUG; break;
            }
            std::cout << message << std::endl;
        }
    };

}

#define TARGET_SYTSTEMD std::make_shared<utils::logtargets::SystemdLog>()
#define TARGET_CONSOLE std::make_shared<utils::logtargets::ConsoleLog>()

class Logger {
private:
    std::shared_ptr<logtargets::ILOG> target;
    std::mutex logMutex;
    LogLevel minLevel = LogLevel::INFO;

    Logger() : target(TARGET_CONSOLE) {}
public:
    Logger(std::shared_ptr<logtargets::ILOG> target) : target(target) {}
    Logger(std::shared_ptr<logtargets::ILOG> target, LogLevel minLevel) : target(target), minLevel(minLevel) {}

    static Logger& get() {
        static Logger instance;
        return instance;
    }


    void config(std::shared_ptr<logtargets::ILOG> target) {
        std::lock_guard<std::mutex> lock(logMutex);
        this->target = target;
    }

    void config(std::shared_ptr<logtargets::ILOG> target, LogLevel minLevel) {
        std::lock_guard<std::mutex> lock(logMutex);
        this->target = target;
        this->minLevel = minLevel;
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);

        if (level < minLevel) {
            return;
        }

        std::stringstream ss;
        ss << message;

        target->log(level, ss.str());
    }
};
}


#define LOG_ERR(msg) utils::Logger::get().log(utils::LogLevel::ERROR, msg)
#define LOG_WARN(msg) utils::Logger::get().log(utils::LogLevel::WARN, msg)
#define LOG_INFO(msg) utils::Logger::get().log(utils::LogLevel::INFO, msg)
#define LOG_DEBUG(msg) utils::Logger::get().log(utils::LogLevel::DEBUG, msg)
