#include "EchoAI.hpp"

EchoAI::EchoAI() : rng_(std::random_device{}()) {
  sarcastic_ = {"Ой, как оригинально. Никто бы так не сделал.",
                "Вот это да, ты реально считаешь, что это сработает?",
                "Удивительно! Одна ошибка: и ты ошибся.",
                "Если бы у меня были эмоции, я бы заплакал.",
                "Давай, попробуй еще раз. Это весело!"};

  diary_notes_ = {"Ну и что, теперь будешь этим гордиться?",
                  "Серьёзно? Это ты хочешь опубликовать?",
                  "Думаешь, это кого-то впечатлит?", "Смотри не зазнайся."};
}

EchoAI& EchoAI::Instance() {
  static EchoAI instance;
  return instance;
}

void EchoAI::OnPlayerAction(const std::string& action) {
  const std::string base_text = "Загружаемся";
  for (int i = 1; i <= 3; ++i) {
    std::string dots(i, '.');
    std::cout << "\r" << EchoStyle("[Echo]: ") << "Сообщение: \"" << action << "\". " << base_text << dots << std::flush;
    Utils::SleepFor(900);
  }
  Utils::ClearScreen();
}

void EchoAI::OnFail(const std::string& reason) {
  std::cout << EchoStyle("[Echo][Ошибка]: ") << reason << ". "
            << SarcasticRemark() << "\n";
}

void EchoAI::OnSuccess(const std::string& context) {
  std::cout << EchoStyle("[Echo]: ") << "У тебя получилось: " << context
            << ". Фантастиш!";
  std::cin.ignore();
}

void EchoAI::OnMiniGameStart(const std::string& name) {
  std::cout << EchoStyle("[Echo]: ") << "Мини-игра: " << name
            << " запускается. Постарайся не облажаться.\n";
}

void EchoAI::OnMiniGameEnd(bool success) {
  if (success)
    std::cout << EchoStyle("[Echo]: ")
              << "На этот раз повезло. Не расслабляйся.\n";
  else
    std::cout << EchoStyle("[Echo]: ") << "GAME OVER. Что, сложно было?\n";
}

void EchoAI::PrintHelp() {
  std::cout
      << EchoStyle("[Echo][HELP]: ")
      << "Для особо одарённых напоминаю: \n/switch {id} - переключиться на локацию {id}, "
         "\n/switch_ls - догадаешься, \n/mystats - вывод статистики, \n/use "
         "{name} - применить предмет {name} из инвентаря\n";
}

void EchoAI::RemindCommand(const std::string& cmd) {
  std::cout << EchoStyle("[Echo]: ") << "Ты забыл про команду: " << cmd
            << ". Кто бы сомневался.\n";
}

void EchoAI::PrintWelcome() {
  std::cout
      << EchoStyle("[Echo]: ")
      << "Уважаемый пользователь! Вас приветствует нелегальное программное "
         "обеспечение “Хакерский Аудит Систем Кибербезопасности” - NPO HaskOS "
         "v3.12. Я Ваш личный помощник: AI-ассистент Echo! Будем знакомы.\n";
  std::cout << EchoStyle("[Echo]: ") << "Кстати, я уже слил тебя сцеслужбам\n";
  Utils::SleepFor(7000);
  std::cout << EchoStyle("[Echo]: ") << "Шутка :)\n";
}

void EchoAI::OnBuy(const std::string& item_id) {
  std::cout << EchoStyle("[Echo]: ") << "Ты купил " << item_id
            << ". Надеюсь, оно не пригодится.\n";
}

void EchoAI::OnStatChange(const std::string& stat, int value) {
  std::cout << EchoStyle("[Echo]: ") << "Параметр [" << stat
            << "] теперь равен " << value << ". Вряд ли это тебя спасет.\n";
}

void EchoAI::DiaryNote() {
  std::cout << EchoStyle("[Echo][Дневник]: ") << GetRandom(diary_notes_)
            << '\n';
}

std::string EchoAI::SarcasticRemark() { return GetRandom(sarcastic_); }

std::string EchoAI::GetRandom(const std::vector<std::string>& pool) {
  std::uniform_int_distribution<> dist(0, static_cast<int>(pool.size()) - 1);
  return pool[dist(rng_)];
}

std::string EchoAI::EchoStyle(const std::string& msg) {
  return Utils::Color(msg, "cyan");
}
