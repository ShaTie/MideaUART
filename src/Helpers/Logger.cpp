#include "Helpers/Logger.hpp"

namespace helpers {

LoggerFn Logger::m_logger;

void Logger::log(int level, const char *const tag, int line, const char *const format, ...) {
  if (m_logger == nullptr)
    return;

  va_list arg;
  va_start(arg, format);
  m_logger(level, tag, line, format, arg);
  va_end(arg);
}

}  // namespace helpers
