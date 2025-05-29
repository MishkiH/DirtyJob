#pragma once
#include <string>

class Console {
public:
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string GRAY;
    
    static const std::string BG_RED;
    static const std::string BG_GREEN;
    static const std::string BG_BLUE;
    
    static const std::string BOLD;
    static const std::string UNDERLINE;
    
    static void init();
    static void clear();
    static std::string colorize(const std::string& text, const std::string& color);
    static void printBox(const std::string& title, const std::string& content);
    static void printProgressBar(int current, int total, int width = 30);
};