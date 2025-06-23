#ifndef ECHO_AI_HPP_
#define ECHO_AI_HPP_

#include <string>
#include <vector>
#include <random>

class EchoAI {
 public:
  static EchoAI& Instance();

  // Комментарии к действиям игрока
  void OnPlayerAction(const std::string& action);
  void OnFail(const std::string& reason);
  void OnSuccess(const std::string& context);
  void OnMiniGameStart(const std::string& name);
  void OnMiniGameEnd(bool success);

  // Подсказки по командам
  void PrintHelp();
  void RemindCommand(const std::string& cmd);
  void PrintWelcome();

  // Реакции на покупки, параметры и события
  void OnBuy(const std::string& item_id);
  void OnStatChange(const std::string& stat, int value);

  // Ироничная заметка к дневнику
  void DiaryNote();

  // Генерация случайных стёбных реплик
  std::string SarcasticRemark();

  // Для стилизации фразы Echo (синим цветом, например)
  static std::string EchoStyle(const std::string& msg);

 private:
  EchoAI(); // Singleton
  std::mt19937 rng_;

  // Список реплик для разных событий
  std::vector<std::string> sarcastic_;
  std::vector<std::string> diary_notes_;

  // Помогает с рандомизацией фраз
  std::string GetRandom(const std::vector<std::string>& pool);
};

#endif  // ECHO_AI_HPP_
