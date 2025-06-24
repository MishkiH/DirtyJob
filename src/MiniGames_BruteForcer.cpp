#include "MiniGames_BruteForcer.hpp"
#include "EchoAI.hpp"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

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
    std::cout << "Введи этот код как можно быстрее: \033[33m" << code << "\033[0m\n";
    std::string input;
    auto start = std::chrono::steady_clock::now();
    std::cin >> input;
    auto elapsed = std::chrono::steady_clock::now() - start;
    if (input != code) {
      EchoAI::Instance().OnFail("Неверный ввод. Ключ сброшен.");
      win = false;
      break;
    }
    if (elapsed > std::chrono::seconds(10)) {
      EchoAI::Instance().OnFail("Слишком долго думал.");
      win = false;
      break;
    }
    std::cout << "Один ключ подобран!\n";
  }
  EchoAI::Instance().OnMiniGameEnd(win);
  return win;
}
