#ifndef LOGGER_H
#define LOGGER_H
#include <string>
namespace app {
class logger {
private:
  enum LogLevel { ERROR, WARNING, INFO };
  static void log(std::string &str, LogLevel level);
  static void write(std::string &str);

public:
  static void log_info(std::string str);
  static void log_warning(std::string str);
  static void log_error(std::string str);
};

} // namespace app
  // p
#endif // !LOGGER_H
