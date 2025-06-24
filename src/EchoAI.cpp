#include "EchoAI.hpp"
#include "Utils.hpp"
#include <iostream>
#include <chrono>
#include <thread>

EchoAI::EchoAI() : rng_(std::random_device{}()) {
  sarcastic_ = {
      "Ой, как оригинально. Никто бы так не сделал.",
      "Вот это да, ты реально считаешь, что это сработает?",
      "Удивительно! Одна ошибка: и ты ошибся.",
      "Если бы у меня были эмоции, я бы заплакал.",
      "Давай, попробуй еще раз. Это весело!"
  };

  diary_notes_ = {
      "Ну и что, теперь будешь этим гордиться?",
      "Серьёзно? Это ты хочешь опубликовать?",
      "Думаешь, это кого-то впечатлит?",
      "Смотри не зазнайся."
  };
}

EchoAI& EchoAI::Instance() {
  static EchoAI instance;
  return instance;
}

void EchoAI::OnPlayerAction(const std::string& action) {
  std::cout << EchoStyle("[Echo]: ") << "Ты попытался. " << action << ". Ну-ну. Удачи\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  Utils::ClearScreen();

}

void EchoAI::OnFail(const std::string& reason) {
  std::cout << EchoStyle("[Echo][Ошибка]: ") << reason << ". " << SarcasticRemark() << "\n";
}

void EchoAI::OnSuccess(const std::string& context) {
  std::cout << EchoStyle("[Echo]: ") << "У тебя получилось: " << context << ". Чудо!\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  Utils::ClearScreen();
}

void EchoAI::OnMiniGameStart(const std::string& name) {
  std::cout << EchoStyle("[Echo]: ") << "Мини-игра: " << name << " запускается. Постарайся не облажаться.\n";
}

void EchoAI::OnMiniGameEnd(bool success) {
  if (success)
    std::cout << EchoStyle("[Echo]: ") << "На этот раз повезло. Не расслабляйся.\n";
  else
    std::cout << EchoStyle("[Echo]: ") << "GAME OVER. Что, сложно было?\n";
}

void EchoAI::PrintHelp() {
  std::cout << EchoStyle("[Echo][HELP]: ") << "Доступные команды: \n/switch {id} - переключиться на локацию {id}, \n/switch_ls - догадаешься, \n/mystats - вывод статистики, \n/use {name} - применить предмет {name} из инвентаря\n";
}

void EchoAI::RemindCommand(const std::string& cmd) {
  std::cout << EchoStyle("[Echo]: ") << "Ты забыл про команду: " << cmd << ". Кто бы сомневался.\n";
}

void EchoAI::PrintWelcome() {
  std::cout << EchoStyle("[Echo]: ") << "Уважаемый пользователь! Вас приветствует нелегальное программное обеспечение “Хакерский Аудит Систем Кибербезопасности” - NPO HaskOS v3.12. Я Ваш личный помощник: AI-ассистент Echo! Будем знакомы.\n";
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << EchoStyle("[Echo}: ") << "Кстати. Ты мне уже не нравишься.\n";
}

void EchoAI::OnBuy(const std::string& item_id) {
  std::cout << EchoStyle("[Echo]: ") << "Ты купил " << item_id << ". Надеюсь, оно не пригодится.\n";
}

void EchoAI::OnStatChange(const std::string& stat, int value) {
  std::cout << EchoStyle("[Echo]: ") << "Параметр [" << stat << "] теперь равен " << value << ". Не то чтобы это тебя спасет.\n";
}

void EchoAI::DiaryNote() {
  std::cout << EchoStyle("[Echo][Дневник]: ") << GetRandom(diary_notes_) << "\n";
}

std::string EchoAI::SarcasticRemark() {
  return GetRandom(sarcastic_);
}

std::string EchoAI::GetRandom(const std::vector<std::string>& pool) {
  std::uniform_int_distribution<> dist(0, static_cast<int>(pool.size()) - 1);
  return pool[dist(rng_)];
}

std::string EchoAI::EchoStyle(const std::string& msg) {
  return Utils::Color(msg, "cyan");
}
