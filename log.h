#pragma once
#include "common.h"

class TimeWrapper;
class EntryExit;

static const char* file_name(const char* path) {
    const char* file = path;
    if (file)
    {
        while (*path) {
            if (*path++ == '/') {
                file = path;
            }
        }
    }

    return file;
}

#define _DEBUG(level,scope,file,line, msg, ...)                                     \
{                                                                                   \
    fprintf(stdout,"[%s] %s:(%d): %s(): ", level, file_name(file), line, scope);    \
    fprintf(stdout, msg, ##__VA_ARGS__); fprintf(stdout, "\n");                     \
}

#define TIME_DEBUG(level,scope,file,line, msg, ...)                                 \
{                                                                                   \
    TimeWrapper time_wrapper;                                                       \
    _DEBUG(level, scope, file, line, msg, ##__VA_ARGS__);                            \
}
#define LOG_DEBUG(level, msg, ...) TIME_DEBUG(level, __func__, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) LOG_DEBUG("ERROR", msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) LOG_DEBUG("INFO", msg, ##__VA_ARGS__)
#define LOG_TRACE(msg, ...) LOG_DEBUG("TRACE", msg, ##__VA_ARGS__)

#define PRINT(msg, ...) {fprintf(stdout, msg, ##__VA_ARGS__); fprintf(stdout, "\n");}

#ifdef TRACE
#define ENTER_BLOCK EntryExit _entryExit(__func__, __FILE__, __LINE__)
#else
#define ENTER_BLOCK
#endif

class TimeWrapper {
public:
    TimeWrapper() {
        PrintCurrentTime();
    }
    ~TimeWrapper() {
    }
private:
    void PrintCurrentTime() {
        std::time_t cnow = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        std::cout << std::put_time(std::localtime(&cnow), "%T ");
    }
};

class EntryExit {
public:
    EntryExit(const char* scope, const char* file, int line = 0)
    :   m_scope(scope), m_file(file), m_line(line) {
        TIME_DEBUG("TRACE", m_scope, m_file, m_line, "Enter")
    }

    ~EntryExit() {
        TIME_DEBUG("TRACE", m_scope, m_file, m_line, "Exit")
    }
private:
    const char* m_scope;
    const char* m_file;
    int m_line;
};
