#include "GameManager.hpp"

GameManager::GameManager()
    : current_ws_(Workspace::Main),
      shop_("../data/shop_items.json"),
      mail_("../data/mail.json"),
      diary_("../data/diary.json") {
  GenerateWorkspaceIds();
  current_ws_id_ = ws_ids_[current_ws_];
}

void GameManager::GenerateWorkspaceIds() {
  std::random_device rd;
  std::mt19937 rng(rd());
  auto gen_id = [&rng]() {
    std::string chars = "abcdef1234567890";
    std::string id;
    for (int i = 0; i < 6; ++i) id += chars[rng() % chars.size()];
    return id;
  };
  ws_ids_[Workspace::Mail] = gen_id();
  ws_ids_[Workspace::Diary] = gen_id();
  ws_ids_[Workspace::Shop] = gen_id();
  ws_ids_[Workspace::Main] = gen_id();
  workspace_map_[ws_ids_[Workspace::Mail]] = Workspace::Mail;
  workspace_map_[ws_ids_[Workspace::Diary]] = Workspace::Diary;
  workspace_map_[ws_ids_[Workspace::Shop]] = Workspace::Shop;
  workspace_map_[ws_ids_[Workspace::Main]] = Workspace::Main;
}

void GameManager::Run() {
  Utils::StartMenu();
  if (DiaryEntry* intro = diary_.FindById(0);
      intro && !intro->published) {
    ShowDiaryEntry(0, true);
    Utils::SleepFor(600);
    Utils::ClearScreen();
  }

  ShowMain();
  EchoAI::Instance().PrintWelcome();

  while (true) {
    ShowPrompt();
    std::string line;
    std::getline(std::cin, line);
    if (line == "/exit") {
      SwitchToWorkspace(Workspace::Main);
      continue;
    }
    if (line.empty()) continue;
    HandleCommand(line);
  }
}

void GameManager::ShowPrompt() const {
  std::cout << Utils::Color("[HaskOS v3.12][" + current_ws_id_ + "] $ ",
                            "green");
}

void GameManager::HandleCommand(const std::string& cmd) {
  std::istringstream iss(cmd);
  std::string token;
  iss >> token;

  if (token == "/switch") {
    HandleSwitchCommand(iss);
  } else if (token == "/switch_ls") {
    ListWorkspaces();
  } else if (token == "/mystats") {
    ShowStats();
  } else if (token == "/select") {
    HandleSelectCommand(iss);
  } else if (awaiting_mail5_choice_) {
    HandleMail5Choice(cmd);
  } else if (awaiting_accept_reject_) {
    HandleAcceptReject(cmd);
  } else if (token == "/use") {
    HandleUseCommand(iss);
  } else if (token == "/buy") {
    HandleBuyCommand(iss);
  } else if (token == "/help") {
    EchoAI::Instance().PrintHelp();
  } else {
    EchoAI::Instance().OnFail("Неизвестная команда.");
    EchoAI::Instance().PrintHelp();
  }
}

void GameManager::HandleSwitchCommand(std::istringstream& iss) {
  std::string id;
  iss >> id;
  if (workspace_map_.count(id)) {
    EchoAI::Instance().OnPlayerAction("Переключение рабочего пространства");
    SwitchToWorkspace(workspace_map_.at(id));
  } else {
    EchoAI::Instance().OnFail("Нет такого пространства");
  }
}

void GameManager::SwitchToWorkspace(Workspace ws) {
  current_ws_ = ws;
  current_ws_id_ = ws_ids_[ws];

  switch (ws) {
    case Workspace::Mail:
      ShowMail();
      break;
    case Workspace::Diary:
      ShowDiary();
      break;
    case Workspace::Shop:
      ShowShop();
      break;
    case Workspace::Main:
      ShowMain();
      break;
  }
}

void GameManager::ListWorkspaces() const {
  Utils::ClearScreen();
  std::cout << Utils::Color("Доступные рабочие пространства:\n", "yellow");
  std::cout << "mail:   " << ws_ids_.at(Workspace::Mail) << "\n";
  std::cout << "diary:  " << ws_ids_.at(Workspace::Diary) << "\n";
  std::cout << "shop:   " << ws_ids_.at(Workspace::Shop) << "\n";
  std::cout << "main:   " << ws_ids_.at(Workspace::Main) << "\n";
  std::cout << EchoAI::EchoStyle("[Echo]: ") +
                   "Вдруг ты забыл: /switch {id} - для перемещения.\n";
}

void GameManager::HandleSelectCommand(std::istringstream& iss) {
  int id;
  iss >> id;

  if (current_ws_ == Workspace::Mail) {
    selected_mail_id_ = id;
    if (id == 5) {
      HandleSpecialMail();
    } else {
      HandleRegularMail();
    }
  } else if (current_ws_ == Workspace::Diary) {
    const DiaryEntry* published = diary_.GetEntryById(id);
    if (!published) {
      EchoAI::Instance().OnFail("Запись не найдена.");
      return;
    }

    ShowDiaryEntry(published->id, false);
    std::cout << Utils::Color("\nНажмите Enter для возврата в дневник...",
                              "cyan");
    std::cin.get();
    ShowDiary();
  } else {
    EchoAI::Instance().OnFail(
        "Команда /select работает только в почте и дневнике.");
  }
}

void GameManager::HandleRegularMail() {
  MailEntry* mail_entry = mail_.GetMailByID(selected_mail_id_);
  if (!mail_entry) {
    EchoAI::Instance().OnFail("Письмо не найдено");
    return;
  }

  ShowMailEntry(selected_mail_id_);
  awaiting_accept_reject_ = true;
  std::cout << "\nДля принятия заказа введите y/n. Для вывода списка /cancel\n";
}

void GameManager::HandleSpecialMail() {
  MailEntry* mail_entry = mail_.GetMailByID(selected_mail_id_);
  if (!mail_entry) {
    EchoAI::Instance().OnFail("Письмо не найдено.");
    return;
  }

  ShowMailEntry(selected_mail_id_);
  awaiting_mail5_choice_ = true;
  ShowMail5Choice();
}

void GameManager::ShowMailEntry(int id) {
  MailEntry* mail_entry = mail_.GetMailByID(id);
  if (!mail_entry) {
    EchoAI::Instance().OnFail("Письмо не найдено.");
    return;
  }

  Utils::ClearScreen();
  std::cout << "Отправитель: " << mail_entry->sender << '\n';
  std::cout << "Тема: " << mail_entry->subject << '\n';
  std::cout << "====================\n";
  std::cout << mail_entry->content << "\n\n";
  std::cout << "Награда за выполнение: " << mail_entry->reward << " ETH\n\n";
}

void GameManager::ShowMail5Choice() {
  int moral_lvl = player_.Moral()->GetValue();
  bool can_refuse = (moral_lvl > -10);
  bool can_accept = (moral_lvl < 10);

  std::cout << Utils::Color(
      "[1] Отказаться от грязной работы (хорошая концовка)\n",
      (can_refuse ? "green" : "black"));
  if (!can_refuse) {
    std::cout << Utils::Color(
        "Причина: Уровень морального диссидента слишком низок для отказа\n",
        "black");
  }

  std::cout << Utils::Color("[2] Принять грязный заказ (плохая концовка)\n",
                            (can_accept ? "red" : "black"));
  if (!can_accept) {
    std::cout << Utils::Color(
        "Причина: Уровень морального диссидента слишком высок для принятия\n",
        "black");
  }

  std::cout << "Моральный диссидент: " << moral_lvl << "\n";
  std::cout << "Выберите вариант (1/2): ";
}

void GameManager::HandleMail5Choice(const std::string& cmd) {
  int choice = 0;
  std::istringstream iss_choice(cmd);
  iss_choice >> choice;

  int moral_lvl = player_.Moral()->GetValue();
  bool can_refuse = (moral_lvl > -10);
  bool can_accept = (moral_lvl < 10);

  if ((choice == 1 && can_refuse) || (choice == 2 && can_accept)) {
    awaiting_mail5_choice_ = false;
    if (choice == 1) {
      ShowDiaryEntryAndEnd(5);
    } else if (choice == 2) {
      StartBadEndGame();
    }
  } else {
    std::cout << "Вариант недоступен! Повторите выбор:\n";
    ShowMail5Choice();
  }
}

void GameManager::HandleAcceptReject(const std::string& cmd) {
  if (cmd == "y") {
    awaiting_accept_reject_ = false;
    if (selected_mail_id_ != -1) StartMissionByID(selected_mail_id_);
    selected_mail_id_ = -1;
  } else if (cmd == "n") {
    awaiting_accept_reject_ = false;
    if (selected_mail_id_ != -1) {
      MailEntry* mail_entry = mail_.GetMailByID(selected_mail_id_);
      if (mail_entry) {
        player_.Moral()->Add(mail_entry->consequence_bad);
        mail_.RejectMail(selected_mail_id_);
        EchoAI::Instance().OnFail("Письмо отклонено.");
      }
    }
    selected_mail_id_ = -1;
    ShowMail();
  } else if (cmd == "/cancel") {
    awaiting_accept_reject_ = false;
    selected_mail_id_ = -1;
    ShowMail();
  } else {
    std::cout << "Введите y/n или /cancel\n";
  }
}

void GameManager::HandleUseCommand(std::istringstream& iss) {
  std::string item_id;
  iss >> item_id;

  if (item_id.empty()) {
    EchoAI::Instance().OnFail("Укажи ID предмета.");
    return;
  }
  shop_.ApplyItem(player_, item_id);
}

void GameManager::HandleBuyCommand(std::istringstream& iss) {
  std::string item_id;
  iss >> item_id;
  if (item_id.empty()) {
    EchoAI::Instance().OnFail("Укажи ID предмета.");
    return;
  }
  shop_.Buy(player_, item_id);
}

void GameManager::ShowMail() {
  Utils::ClearScreen();
  mail_.ShowInbox();
  std::cout << Utils::Color(
      "Доступные команды: /select {id} - выбор конкретного письма, /exit - "
      "выход в главное меню\n",
      "cyan");
}

void GameManager::ShowDiary() {
  Utils::ClearScreen();
  diary_.Show();
  std::cout << Utils::Color(
      "Доступные команды: /select {id} - вспомнить запись, /exit - выход в "
      "главное меню\n",
      "cyan");
}

void GameManager::ShowShop() {
  Utils::ClearScreen();
  shop_.Show();
  std::cout << Utils::Color(
      "Доступные команды: /buy {name} - приобрести предмет, /exit - выход в "
      "главное меню\n",
      "cyan");
}

void GameManager::ShowMain() {
  Utils::ClearScreen();
  std::cout << Utils::Color(
      "[Представьте фото котёнка в корзинке, на фоне растёт зелёный лужок. Это "
      "ваши обои.]\n",
      "green");
  std::cout << Utils::Color("Святая всех святых: твоё рабочее место.\n",
                            "blue");
  std::cout << Utils::Color(
      "Доступные команды: /switch_ls, /switch {id}, /mystats\n", "blue");
}

void GameManager::ShowStats() {
  auto* moral = player_.Moral();
  auto* health = player_.Health();
  std::cout << Utils::Color("Параметры игрока:\n", "yellow");
  std::cout << "  Моральный диссидент: " << (moral ? moral->GetValue() : 0)
            << "\n";
  std::cout << "  Степень защиты: " << (health ? health->Get() : 0) << "/"
            << (health ? health->Max() : 0) << "\n";
  std::cout << "  Баланс ETH: " << player_.Inventory()->GetCount("ETH") << "\n";
}

void GameManager::StartMissionByID(int mail_id) {
  MailEntry* mail_entry = mail_.GetMailByID(mail_id);
  if (!mail_entry) {
    EchoAI::Instance().OnFail("Письма с таким ID нет");
    return;
  }

  EchoAI::Instance().OnPlayerAction("Принятие заказа: " + mail_entry->subject);

  bool success = false;
  if (mail_entry->id == 1 || mail_entry->id == 2) {
    current_mini_game_ =
        std::make_unique<FWByPass>("../MiniGames/fwby_maze.json");
    success = current_mini_game_->Play();
  } else if (mail_entry->id == 3 || mail_entry->id == 4) {
    current_mini_game_ = std::make_unique<BruteForcer>();
    success = current_mini_game_->Play();
  } else {
    return;
  }

  if (success) {
    mail_entry->finished = true;
    mail_.RejectOtherMailsInCurrentWave(mail_entry->id);
    EchoAI::Instance().OnSuccess(mail_entry->subject);
    Utils::ClearScreen();
    player_.Inventory()->AddItem("ETH", mail_entry->reward);
    auto* moral = player_.Moral();
    if (moral)
      moral->Add(mail_entry->good ? mail_entry->consequence_good
                                  : mail_entry->consequence_bad);

    DiaryEntry* found_entry = diary_.FindById(mail_entry->id);
    if (found_entry && !found_entry->published) {
      ShowDiaryEntry(mail_entry->id, true);
    }
    Utils::ClearScreen();
    ShowMain();
  } else {
    auto* health = player_.Health();
    if (health) {
      health->Damage(1);
      EchoAI::Instance().OnStatChange("HP", health->Get());
      if (health->Get() <= 0) {
        GameOver();
      }
    }
  }
}

void GameManager::ShowDiaryEntry(int id, bool show_prompt) {
  DiaryEntry* entry = diary_.FindById(id);

  Utils::ClearScreen();
  std::cout << Utils::Color(
      "Запись №" + std::to_string(*entry->mission_id + 1) + ". Конспект.\n",
      "magenta");
  std::cout << Utils::Color("\"" + entry->title + "\"\n\n", "yellow");
  Utils::PrintSlowlyByChar(entry->text);

  std::cout << Utils::Color("\nВлияние на морального диссидента: ", "yellow");
  if (entry->moral_effect > 0) std::cout << "+";
  std::cout << entry->moral_effect << '\n';

  if (show_prompt) {
    std::cout << "\nY:[Опубликовать] | N:[Удалить]\n";
    char ans;
    std::cin >> ans;
    DiaryEntry* published = diary_.FindById(id);
    if (published) {
      if (ans == 'Y' || ans == 'y') {
        published->published = true;
        if (auto* moral = player_.Moral()) {
          moral->Add(published->moral_effect);
        }
      } else {
        if (auto* moral = player_.Moral()) {
          moral->Add(-published->moral_effect);
        }
      }
    }
  }
}

void GameManager::StartBadEndGame() {
  current_mini_game_ = std::make_unique<ProtocolSimon>();
  bool success = current_mini_game_->Play();
  (void)success;  // успех невозможен
  ShowDiaryEntryAndEnd(6);
}

void GameManager::ShowDiaryEntryAndEnd(int id) {
  ShowDiaryEntry(id, true);
  Utils::SleepFor(600);
  std::cout << Utils::Color("To be continued..\n", "cyan");
  std::cin.ignore();
  std::exit(0);
}

void GameManager::GameOver() {
  std::cout << Utils::Color("GAME OVER. Press any key to try over.\n", "red");
  std::string dummy;
  std::getline(std::cin, dummy);
  auto* health = player_.Health();
  auto* moral = player_.Moral();
  if (health) *health = HealthComponent(2);
  if (moral) *moral = MoralComponent(0);
  player_.Inventory()->AddItem("ETH", 0);
  EchoAI::Instance().OnPlayerAction("Давай по новой, всё брехня");
}