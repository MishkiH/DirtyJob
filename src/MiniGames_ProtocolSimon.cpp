#include "MiniGames_ProtocolSimon.hpp"
#include "EchoAI.hpp"
#include <iostream>
#include <vector>
#include <random>

// Упрощённый вариант — просто повторить последовательность
bool ProtocolSimon::Play() {
  EchoAI::Instance().OnMiniGameStart(Name());
  std::vector<int> seq;
  int max_rounds = 5;
  int nodes = 9;
  std::random_device rd;
  std::mt19937 rng(rd());
  bool win = true;
  for (int r = 0; r < max_rounds; ++r) {
    seq.push_back(rng() % nodes + 1);
    std::cout << "Последовательность: ";
    for (int i : seq) std::cout << i << " ";
    std::cout << "\nВведите её (через пробел): ";
    for (int i = 0; i < seq.size(); ++i) {
      int user;
      std::cin >> user;
      if (user != seq[i]) {
        EchoAI::Instance().OnFail("Ошибка! Последовательность сброшена.");
        win = false;
        goto finish;
      }
    }
    std::cout << "Раунд пройден!\n";
  }
finish:
  EchoAI::Instance().OnMiniGameEnd(win);
  return win;
}
