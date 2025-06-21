#include "MiniGames_ProtocolSimon.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#define CLEAR() system("cls")
#else
#include <unistd.h>
#define CLEAR() system("clear")
#endif

// ANSI цвет для терминала
std::string Color(char key) {
    switch (key) {
    case 'W': return "\033[1;34m"; // Синий
    case 'A': return "\033[1;32m"; // Зелёный
    case 'S': return "\033[1;31m"; // Красный
    case 'D': return "\033[1;33m"; // Жёлтый
    default: return "\033[0m";
    }
}

void ShowChar(char c) {
    CLEAR();
    std::cout << "Смотри внимательно:\n\n";
    std::cout << Color(c) << c << "\033[0m" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    CLEAR();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

bool ProtocolSimon::Play() {
    const int max_rounds = 7;

    std::vector<char> keys = { 'W', 'A', 'S', 'D' };
    std::vector<char> sequence;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, keys.size() - 1);

    std::cout << "=== Протокол SIMON ===\n";
    std::cout << "Запоминай последовательность мигающих клавиш (WASD) и вводи её.\n";
    std::cout << "Успешный взлом после 7 раундов!\n";
    std::cout << "Нажми Enter, чтобы начать...";
    std::cin.get();

    int round = 1;

    while (round <= max_rounds) {
        sequence.push_back(keys[dist(gen)]);

        for (char c : sequence) {
            ShowChar(c);
        }

        std::cout << "Введи последовательность (например: WAS): ";
        std::string input;
        std::cin >> input;

        for (char& ch : input) ch = std::toupper(ch);

        bool correct = true;
        if (static_cast<size_t>(input.length()) != sequence.size()) {
            correct = false;
        }
        else {
            for (size_t i = 0; i < sequence.size(); ++i) {
                if (input[i] != sequence[i]) {
                    correct = false;
                    break;
                }
            }
        }

        if (!correct) {
            std::cout << "\n Ошибка! Правильная последовательность была: ";
            for (char c : sequence) std::cout << c;
            std::cout << "\nИгра окончена. Ты прошёл " << round - 1 << " раунд(ов).\n";
            return false;
        }
        else {
            if (round < max_rounds) {
                std::cout << " Верно! Переход к раунду " << (round + 1) << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            ++round;
        }
    }

    std::cout << "\n ВЗЛОМ УСПЕШЕН! Ты прошёл все 7 раундов.\n";
    return true;
}
