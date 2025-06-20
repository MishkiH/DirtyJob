#include "EchoAI.hpp"
#include <iostream>

EchoAI::EchoAI() : rng_(std::random_device{}()) {
  sarcastic_ = {
      "Ой, как оригинально. Никто бы так не сделал.",
      "Вот это да, ты реально считаешь, что это сработает?",
      "Удивительно! Еще одна ошибка. Ты стабилен как никогда.",
      "Если бы у меня были эмоции, я бы сейчас смеялся.",
      "Давай, попробуй еще раз. Это весело!"
  };

  diary_notes_ = {
      "Ну и что, теперь будешь этим гордиться?",
      "Серьёзно? Это ты хочешь опубликовать?",
      "Думаешь, это кого-то впечатлит?",
      "Твой дневник плачет кровавыми слезами."
  };
}

EchoAI& EchoAI::Instance() {
  static EchoAI instance;
  return instance;
}

void EchoAI::OnPlayerAction(const std::string& action) {
  std::cout << EchoStyle("[Echo]: ") << "Ты попытался: " << action << ". Ну-ну.\n";
}

void EchoAI::OnFail(const std::string& reason) {
  std::cout << EchoStyle("[Echo][Ошибка]: ") << reason << ". " << SarcasticRemark() << "\n";
}

void EchoAI::OnSuccess(const std::string& context) {
  std::cout << EchoStyle("[Echo]: ") << "У тебя получилось: " << context << ". Чудо!\n";
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
  std::cout << EchoStyle("[Echo][HELP]: ") << "Доступные команды: /switch, /switch_ls, /take {num}, /use {ID}, /mystats, /shop, /diary, /mail, /help\n";
}

void EchoAI::RemindCommand(const std::string& cmd) {
  std::cout << EchoStyle("[Echo]: ") << "Ты забыл про команду: " << cmd << ". Не удивительно.\n";
}

void EchoAI::PrintWelcome() {
  std::cout << EchoStyle("[Echo]: ") << "Добро пожаловать в грязную работу. Я — Echo. Привыкай, мы теперь вместе надолго.\n";
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
  return "\033[36m" + msg + "\033[0m"; // cyan
}
