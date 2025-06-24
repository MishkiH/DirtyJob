#ifndef ECHO_AI_HPP_
#define ECHO_AI_HPP_

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "Utils.hpp"

class EchoAI {
 public:
  static EchoAI& Instance();

  // комментарии к действиям игрока
  void OnPlayerAction(const std::string& action);
  void OnFail(const std::string& reason);
  void OnSuccess(const std::string& context);
  void OnMiniGameStart(const std::string& name);
  void OnMiniGameEnd(bool success);

  // подсказки по командам
  void PrintHelp();
  void RemindCommand(const std::string& cmd);
  void PrintWelcome();

  // реакции на покупки, параметры и события
  void OnBuy(const std::string& item_id);
  void OnStatChange(const std::string& stat, int value);

  // ироничная заметка к дневнику
  void DiaryNote();

  // генерация случайных стёбных реплик
  std::string SarcasticRemark();

  // для стилизации фразы Echo (синим цветом, например)
  static std::string EchoStyle(const std::string& msg);

 private:
  EchoAI();  // Singleton
  std::mt19937 rng_;

  // список реплик для разных событий
  std::vector<std::string> sarcastic_;
  std::vector<std::string> diary_notes_;

  // помогает с рандомизацией фраз
  std::string GetRandom(const std::vector<std::string>& pool);
};

#endif  // ECHO_AI_HPP_
