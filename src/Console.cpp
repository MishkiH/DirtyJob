#include "../include/Console.hpp"
#include <iostream>
#include <cstdlib>

const std::string Console::RESET = "\033[0m";
const std::string Console::RED = "\033[31m";
const std::string Console::GREEN = "\033[32m";
const std::string Console::YELLOW = "\033[33m";
const std::string Console::BLUE = "\033[34m";
const std::string Console::MAGENTA = "\033[35m";
const std::string Console::CYAN = "\033[36m";
const std::string Console::WHITE = "\033[37m";
const std::string Console::GRAY = "\033[90m";

const std::string Console::BG_RED = "\033[41m";
const std::string Console::BG_GREEN = "\033[42m";
const std::string Console::BG_BLUE = "\033[44m";

const std::string Console::BOLD = "\033[1m";
const std::string Console::UNDERLINE = "\033[4m";

void Console::init() {  //инициализация консоли для поддержки ANSI-кодов в винде
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
}

void Console::clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string Console::colorize(const std::string& text, const std::string& color) {
    return color + text + RESET;
}

void Console::printBox(const std::string& title, const std::string& content) {
    size_t width = std::max(title.length(), content.length()) + 4;
    
    std::cout << colorize("╔", CYAN);
    for (size_t i = 0; i < width; ++i) {
        std::cout << colorize("═", CYAN);
    }
    std::cout << colorize("╗\n", CYAN);
    
    std::cout << colorize("║ ", CYAN) << colorize(title, YELLOW);
    for (size_t i = title.length(); i < width - 1; ++i) {
        std::cout << " ";
    }
    std::cout << colorize("║\n", CYAN);
    
    std::cout << colorize("╠", CYAN);
    for (size_t i = 0; i < width; ++i) {
        std::cout << colorize("═", CYAN);
    }
    std::cout << colorize("╣\n", CYAN);
    
    std::cout << colorize("║ ", CYAN) << content;
    for (size_t i = content.length(); i < width - 1; ++i) {
        std::cout << " ";
    }
    std::cout << colorize("║\n", CYAN);
    
    std::cout << colorize("╚", CYAN);
    for (size_t i = 0; i < width; ++i) {
        std::cout << colorize("═", CYAN);
    }
    std::cout << colorize("╝\n", CYAN);
}

void Console::printProgressBar(int current, int total, int width) {
    float progress = static_cast<float>(current) / total;
    int filled = static_cast<int>(progress * width);
    
    std::cout << colorize("[", WHITE);
    for (int i = 0; i < width; ++i) {
        if (i < filled) {
            if (progress > 0.7f) {
                std::cout << colorize("█", GREEN);
            } else if (progress > 0.3f) {
                std::cout << colorize("█", YELLOW);
            } else {
                std::cout << colorize("█", RED);
            }
        } else {
            std::cout << colorize("░", GRAY);
        }
    }
    std::cout << colorize("]", WHITE);
}