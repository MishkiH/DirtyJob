#include "MiniGames_ProtocolSimon.hpp"

void ShowChar(char c) {
  Utils::ClearScreen();
  std::cout << "Смотри внимательно:\n\n";
  switch (c) {
    case 'W':
      std::cout << Utils::Color(std::string(1, c), "blue");
      break;
    case 'A':
      std::cout << Utils::Color(std::string(1, c), "green");
      break;
    case 'S':
      std::cout << Utils::Color(std::string(1, c), "red");
      break;
    case 'D':
      std::cout << Utils::Color(std::string(1, c), "yellow");
      break;
  }
  Utils::SleepFor(600 * Shop::coffeeGag);
  Utils::ClearScreen();
  Utils::SleepFor(300 * Shop::coffeeGag);
}

bool ProtocolSimon::Play() {
  std::vector<char> keys = {'W', 'A', 'S', 'D'};
  std::vector<char> sequence;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, keys.size() - 1);

  Utils::SleepFor(3000);
  Utils::ClearScreen();
  std::cout << Utils::Color("=== ProtocolSimon ===\n", "yellow");
  std::cout
      << "Запоминай последовательность мигающих клавиш (WASD) и вводи её! :)\n";
  std::cout << "Взлом продлится до.. первой ошибки! АХ-ХА-ХА-ХАА\n";
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
