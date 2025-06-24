#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <conio.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "windows.h"

namespace Utils {

void SleepFor(const int& ms);
void ClearScreen();
std::string Color(const std::string& text, const std::string& color);

void PrintSlowlyByChar(const std::string& text);
}  // namespace Utils

#endif  // UTILS_HPP_
