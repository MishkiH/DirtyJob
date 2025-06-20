#include "Utils.hpp"

namespace Utils {

std::string Color(const std::string& text, const std::string& color) {
  if (color == "red")    return "\033[31m" + text + "\033[0m";
  if (color == "green")  return "\033[32m" + text + "\033[0m";
  if (color == "yellow") return "\033[33m" + text + "\033[0m";
  if (color == "blue")   return "\033[34m" + text + "\033[0m";
  if (color == "magenta")return "\033[35m" + text + "\033[0m";
  if (color == "cyan")   return "\033[36m" + text + "\033[0m";
  return text;
}

} // namespace Utils
