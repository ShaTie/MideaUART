#pragma once

#include <cstdarg>
#include <functional>

namespace helpers {

using LoggerFn = std::function<void(int, const char *, int, const char *, va_list)>;

class Logger {
 public:
  static void set(LoggerFn logger) { m_logger = logger; }
  static void log(int level, const char *tag, int line, const char *format, ...);

 private:
  static LoggerFn m_logger;
};

}  // namespace helpers

using helpers::Logger;

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_CONFIG 4
#define LOG_LEVEL_DEBUG 5
#define LOG_LEVEL_VERBOSE 6
#define LOG_LEVEL_VERY_VERBOSE 7

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERY_VERBOSE
#define sv_log_vv(tag, format, ...) Logger::log(LOG_LEVEL_VERY_VERBOSE, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_vv(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define sv_log_v(tag, format, ...) Logger::log(LOG_LEVEL_VERBOSE, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_v(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define sv_log_d(tag, format, ...) Logger::log(LOG_LEVEL_DEBUG, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_d(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_CONFIG
#define sv_log_config(tag, format, ...) Logger::log(LOG_LEVEL_CONFIG, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_config(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define sv_log_i(tag, format, ...) Logger::log(LOG_LEVEL_INFO, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_i(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define sv_log_w(tag, format, ...) Logger::log(LOG_LEVEL_WARN, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_w(tag, format, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define sv_log_e(tag, format, ...) Logger::log(LOG_LEVEL_ERROR, tag, __LINE__, format, ##__VA_ARGS__)
#else
#define sv_log_e(tag, format, ...)
#endif

#define LOG_E(tag, ...) sv_log_e(tag, __VA_ARGS__)
#define LOG_W(tag, ...) sv_log_w(tag, __VA_ARGS__)
#define LOG_I(tag, ...) sv_log_i(tag, __VA_ARGS__)
#define LOG_D(tag, ...) sv_log_d(tag, __VA_ARGS__)
#define LOG_CONFIG(tag, ...) sv_log_config(tag, __VA_ARGS__)
#define LOG_V(tag, ...) sv_log_v(tag, __VA_ARGS__)
#define LOG_VV(tag, ...) sv_log_vv(tag, __VA_ARGS__)
