#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <conio.h>

namespace Utils {

void ClearScreen();
std::string Color(const std::string& text, const std::string& color);

}

inline void PrintSlowlyByChar(const std::string& text, unsigned ms = 30) {
    bool skip = false;
    for (char ch : text) {
        if (!skip && _kbhit()) {             // если нажата клавиша
            char c = _getch();
            if (c == '\r' || c == '\n') {   // если это Enter
                skip = true;
            }
        }
        std::cout << ch << std::flush;
        if (!skip) {
            if (ch == '\n')
                std::this_thread::sleep_for(std::chrono::milliseconds(ms * 4));
            else if (ch == ' ')
                std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(ms / 2));
        }
    }
    std::cout << '\n';
}

#endif  // UTILS_HPP_
