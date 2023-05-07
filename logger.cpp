#include "logger.h"
#include <iostream>

namespace app {
void logger::log(std::string &str, logger::LogLevel level) {

  std::string msg;
  switch (level) {
  case ERROR:
    msg = "[ERROR]" + str;
    logger::write(msg);
    exit(1);
    break;
  case INFO:
    msg = "[INFO]" + str;
    logger::write(msg);
    break;
  case WARNING:
    msg = "[WARNING]" + str;
    logger::write(msg);
    break;
  }
};

void logger::log_info(std::string str) { logger::log(str, logger::INFO); }
void logger::log_error(std::string str) { logger::log(str, logger::ERROR); }
void logger::log_warning(std::string str) { logger::log(str, logger::WARNING); }
void logger::write(std::string &str) { std::cout << str << std::endl; }
} // namespace app
