#include "MiniGames_BruteForcer.hpp"

bool BruteForcer::Play() {
  EchoAI::Instance().OnMiniGameStart(Name());
  std::string chars = "ABCDEF123456";
  int rounds = 3;
  bool win = true;
  for (int i = 0; i < rounds; ++i) {
    std::string code;
    for (int k = 0; k < 8; ++k) {
      code += chars[rand() % chars.size()];
    }
    std::cout << "Введи этот код как можно быстрее:"
              << Utils::Color(code, "green") << '\n';
    std::string input;
    auto start = std::chrono::steady_clock::now();
    std::cin >> input;
    auto elapsed = std::chrono::steady_clock::now() - start;
    if (input != code) {
      EchoAI::Instance().OnFail("Неверный ввод. Ключ сброшен.");
      win = false;
      break;
    }
    if (elapsed > std::chrono::seconds(8 * GameManager::coffeeGag)) {
      EchoAI::Instance().OnFail("Не хочу тебя обижать. Ты тормоз");
      win = false;
      break;
    }
    std::cout << "Ключ подобран!\n";
  }
  EchoAI::Instance().OnMiniGameEnd(win);
  return win;
}
