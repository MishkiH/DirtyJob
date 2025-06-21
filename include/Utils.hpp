#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>

namespace Utils {

void ClearScreen();
std::string Color(const std::string& text, const std::string& color);

}

// Печать текста по словам (с задержкой между словами)
inline void PrintSlowlyByChar(const std::string& text, unsigned ms = 30) {
    for (char ch : text) {
        std::cout << ch << std::flush;
        if (ch == '\n')
            std::this_thread::sleep_for(std::chrono::milliseconds(ms * 4)); // задержка больше для новой строки
        else if (ch == ' ')
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));     // чуть медленнее на пробелах
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(ms / 2)); // обычная задержка
    }
    std::cout << std::endl;
}

#endif  // UTILS_HPP_
