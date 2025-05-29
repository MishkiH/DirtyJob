#include "../../include/components/HasStats.hpp"
#include "../../include/Console.hpp"
#include <iostream>

HasStats::HasStats() 
    : criticalThinking(50), pragmatism(50), rebelSpirit(50), health(100) {
}

int HasStats::clamp(int value, int min, int max) const {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void HasStats::displayStats() const {
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                        ХАРАКТЕРИСТИКИ                        ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    
    std::cout << "\n";
    
    std::cout << Console::colorize("💛 Здоровье: ", Console::RED);
    Console::printProgressBar(health, 100);
    std::cout << " " << health << "/100\n";
    
    std::cout << Console::colorize("🧠 Критическое мышление: ", Console::BLUE);
    Console::printProgressBar(criticalThinking, 100);
    std::cout << " " << criticalThinking << "/100\n";
    
    std::cout << Console::colorize("💼 Прагматизм: ", Console::GREEN);
    Console::printProgressBar(pragmatism, 100);
    std::cout << " " << pragmatism << "/100\n";
    
    std::cout << Console::colorize("⚡ Бунтарский дух: ", Console::MAGENTA);
    Console::printProgressBar(rebelSpirit, 100);
    std::cout << " " << rebelSpirit << "/100\n";
    
    std::cout << "\n";
    
    std::cout << Console::colorize("Профиль хакера:\n", Console::YELLOW);
    
    if (criticalThinking > 70) {
        std::cout << "• Острый ум, видит скрытые угрозы\n";
    } else if (criticalThinking < 30) {
        std::cout << "• Склонен к импульсивным решениям\n";
    }
    
    if (pragmatism > 70) {
        std::cout << "• Холодный расчет, работа за деньги\n";
    } else if (pragmatism < 30) {
        std::cout << "• Движим чувством справедливости\n";
    }
    
    if (rebelSpirit > 70) {
        std::cout << "• Игнорирует правила, бросает вызов системе\n";
    } else if (rebelSpirit < 30) {
        std::cout << "• Осторожен, работает в рамках закона\n";
    }