#include "MiniGames_BruteForcer.hpp"

namespace {

bool input_received = false;
std::string user_input;

void ReadInput() {
  std::cin >> user_input;
  input_received = true;
}

bool InputWithTimeout(int timeout_seconds, std::string& result) {
  input_received = false;
  user_input.clear();

  std::thread input_thread(ReadInput);

  for (int i = 0; i < timeout_seconds * 10; ++i) {
    if (input_received) break;
    Sleep(100);  // проверка каждые 100 ms
  }

  if (input_received) {
    result = user_input;
    input_thread.detach();
    return true;
  } else {
    std::cout << "\nВремя вышло!\n";
    input_thread.detach();
    return false;
  }
}

}  // namespace

bool BruteForcer::Play() {
  EchoAI::Instance().OnMiniGameStart(Name());

  const std::string kCharset = "ABCDEF123456";
  const int kRounds = 3;
  const int kCodeLength = 8;
  bool win = true;

  for (int round = 0; round < kRounds; ++round) {
    std::string code;
    for (int i = 0; i < kCodeLength; ++i) {
      code += kCharset[rand() % kCharset.size()];
    }

    std::cout << "Введи этот код как можно быстрее: "
              << Utils::Color(code, "green") << '\n';

    std::string input;
    int timeout = 8 * Shop::coffeeGag;

    bool success = InputWithTimeout(timeout, input);

    if (!success) {
      EchoAI::Instance().OnFail("Не хочу тебя обижать. Ты тормоз.");
      win = false;
      break;
    }

    if (input != code) {
      EchoAI::Instance().OnFail("Неверный ввод. Ключ сброшен.");
      win = false;
      break;
    }

    std::cout << "Ключ подобран!\n";
  }

  EchoAI::Instance().OnMiniGameEnd(win);
  return win;
}
