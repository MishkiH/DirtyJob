#include "MiniGames_ProtocolSimon.hpp"

#include <windows.h>

#include "Utils.hpp"

// ANSI цвет для терминала
std::string Color(char key) {
  switch (key) {
    case 'W':
      return "\033[1;34m";  // Синий
    case 'A':
      return "\033[1;32m";  // Зелёный
    case 'S':
      return "\033[1;31m";  // Красный
    case 'D':
      return "\033[1;33m";  // Жёлтый
    default:
      return "\033[0m";
  }
}

void ShowChar(char c) {
  Utils::ClearScreen();
  std::cout << "Смотри внимательно:\n\n";
  std::cout << Color(c) << c << "\033[0m" << '\n';
  Utils::SleepFor(600);
  Utils::ClearScreen();
  Utils::SleepFor(300);
}

bool ProtocolSimon::Play() {
  std::vector<char> keys = {'W', 'A', 'S', 'D'};
  std::vector<char> sequence;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, keys.size() - 1);

  std::cout << "=== ProtocolSimon ===\n";
  std::cout
      << "Запоминай последовательность мигающих клавиш (WASD) и вводи её! :)\n";
  std::cout << "Игра идёт до.. первой ошибки! АХ-ХА-ХА-ХАА\n";
  std::cout << "Нажми Enter, чтобы начать ⸜( *ˊᵕˋ* )⸝";
  std::cin.get();

  int round = 1;

  while (true) {
    sequence.push_back(keys[dist(gen)]);
    for (char c : sequence) {
      ShowChar(c);
    }

    std::cout << "Раунд " << round
              << ". Введи получившуюся последовательность (например: SAD): ";
    std::string input;
    std::cin >> input;

    for (char& ch : input) ch = std::toupper(ch);

    bool correct = true;
    if (static_cast<size_t>(input.length()) != sequence.size()) {
      correct = false;
    } else {
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
      Utils::SleepFor(4000);
      std::cout << Utils::Color("\n[Security System]: ", "red")
                << "Игры кончились. Наши специалисты уже взялись за тебя. Даже "
                   "не думай пробовать скрыться. Всё равно найдём ХА-ХА-ХА\n";
      Utils::SleepFor(6000);
      Utils::ClearScreen();
      return false;
    }
    ++round;
  }
  // the win will be unreachable :))))))))))))))
}
