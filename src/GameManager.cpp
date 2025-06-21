#include "GameManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <random>

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
    for (int i = 0; i < 6; ++i)
      id += chars[rng() % chars.size()];
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
  EchoAI::Instance().PrintWelcome();
  EchoAI::Instance().PrintHelp();
  bool running = true;
  while (running) {
    ShowPrompt();
    std::string line;
    std::getline(std::cin, line);
    if (line == "/exit") break;
    if (line.empty()) continue;
    HandleCommand(line);
  }
}

void GameManager::ShowPrompt() const {
  std::cout << Utils::Color("[DirtyJob][" + current_ws_id_ + "] $ ", "green");
}

void GameManager::HandleCommand(const std::string& cmd) {
  std::istringstream iss(cmd);
  std::string token;
  iss >> token;

  if (token == "/switch") {
    std::string id;
    iss >> id;
    if (workspace_map_.count(id)) {
      current_ws_ = workspace_map_.at(id);
      current_ws_id_ = id;
      EchoAI::Instance().OnPlayerAction("переключение рабочего пространства");
      switch (current_ws_) {
        case Workspace::Mail: ShowMail(); break;
        case Workspace::Diary: ShowDiary(); break;
        case Workspace::Shop: ShowShop(); break;
        case Workspace::Main: ShowMain(); break;
      }
    } else {
      EchoAI::Instance().OnFail("Нет такого пространства");
    }
  }
  else if (token == "/switch_ls") {
    ListWorkspaces();
  }
  else if (token == "/mail") {
    ShowMail();
  }
  else if (token == "/diary") {
    ShowDiary();
  }
  else if (token == "/shop") {
    ShowShop();
  }
  else if (token == "/main") {
    ShowMain();
  }
  else if (token == "/mystats") {
    ShowStats();
  }
  else if (token == "/take") {
    int num;
    iss >> num;
    if (num >= 1) StartMissionByID(num);
    else EchoAI::Instance().OnFail("Не указан номер письма");
  }
  else if (token == "/select") {
    int id;
    iss >> id;
    MailEntry* mail_entry = mail_.GetMailByID(id);
    if (!mail_entry) {
      EchoAI::Instance().OnFail("Письмо не найдено.");
      return;
    }
    selected_mail_id_ = id;
    awaiting_accept_reject_ = true;

    Utils::ClearScreen();
    std::cout << "Отправитель: " << mail_entry->sender << std::endl;
    std::cout << "Тема: " << mail_entry->subject << std::endl;
    std::cout << "===========\n";
    std::cout << mail_entry->content << std::endl;
    std::cout << "\nДля принятия заказа введите y/n. Для отмены введите /cancel\n";
    return;
}
else if (awaiting_accept_reject_) {
    if (cmd == "y") {
      awaiting_accept_reject_ = false;
      if (selected_mail_id_ != -1)
        StartMissionByID(selected_mail_id_); // нужна новая функция
      selected_mail_id_ = -1;
      return;
    } else if (cmd == "n") {
        awaiting_accept_reject_ = false;
        if (selected_mail_id_ != -1) {
            MailEntry* mail_entry = mail_.GetMailByID(selected_mail_id_);
            if (mail_entry) {
                player_.Moral()->Add(mail_entry->consequence_bad);
                mail_.RejectMail(selected_mail_id_);
                EchoAI::Instance().OnFail("Письмо отклонено."); //todo: MoralDecedent +-
            }
        }
        selected_mail_id_ = -1;
        ShowMail();
        return;
    } else if (cmd == "/cancel") {
        awaiting_accept_reject_ = false;
        selected_mail_id_ = -1;
    } else {
      std::cout << "Введите y/n или /cancel\n";
      return;
    }
}
  else if (token == "/use") {
    std::string item_id;
    iss >> item_id;
    if (item_id.empty()) {
      EchoAI::Instance().OnFail("Укажи ID предмета.");
      return;
    }
    shop_.ApplyItem(player_, item_id);
  }
  else if (token == "/help") {
    EchoAI::Instance().PrintHelp();
  }
  else {
    EchoAI::Instance().OnFail("Неизвестная команда.");
    EchoAI::Instance().PrintHelp();
  }
}

void GameManager::ListWorkspaces() const {
  std::cout << "Доступные рабочие пространства:\n";
  std::cout << "mail:   " << ws_ids_.at(Workspace::Mail) << "\n";
  std::cout << "diary:  " << ws_ids_.at(Workspace::Diary) << "\n";
  std::cout << "shop:   " << ws_ids_.at(Workspace::Shop) << "\n";
  std::cout << "main:   " << ws_ids_.at(Workspace::Main) << "\n";
}

GameManager::Workspace GameManager::WorkspaceById(const std::string& id) const {
  auto it = workspace_map_.find(id);
  if (it != workspace_map_.end()) return it->second;
  return Workspace::Main;
}

void GameManager::ShowMail() {
  Utils::ClearScreen();
  mail_.ShowInbox();
}

void GameManager::ShowDiary() {
  diary_.Show();
}

void GameManager::ShowShop() {
  shop_.Show();
}

void GameManager::ShowMain() {
  std::cout << Utils::Color("Твоё рабочее место. /switch_ls — список пространств, /mail, /diary, /shop — переходы. /mystats — параметры игрока.\n", "blue");
}

void GameManager::ShowStats() {
  auto* moral = player_.Moral();
  auto* health = player_.Health();
  std::cout << Utils::Color("Параметры игрока:\n", "yellow");
  std::cout << "  Моральный диссидент: " << (moral ? moral->GetValue() : 0) << "\n";
  std::cout << "  Здоровье: " << (health ? health->Get() : 0) << "/" << (health ? health->Max() : 0) << "\n";
  std::cout << "  ETH: " << player_.Inventory()->GetCount("ETH") << "\n";
}

void GameManager::StartMissionByID(int mail_id) {
  MailEntry* mail_entry = mail_.GetMailByID(mail_id);
  if (!mail_entry) {
    EchoAI::Instance().OnFail("Письма с таким ID нет");
    return;
  }
  EchoAI::Instance().OnPlayerAction("Принятие заказа: " + mail_entry->subject);

  // Выбор мини-игры по сценарию
  bool success = false;
  if (mail_entry->id == 1) { // Пример для первой миссии
    current_mini_game_ = std::make_unique<FWByPass>("../MiniGames/fwby_maze.json");
    success = current_mini_game_->Play();
  } else if (mail_entry->id == 2) {
    current_mini_game_ = std::make_unique<BruteForcer>();
    success = current_mini_game_->Play();
  } else if (mail_entry->id == 3) {
    current_mini_game_ = std::make_unique<ProtocolSimon>();
    success = current_mini_game_->Play();
  } else {
    // fallback
    success = true;
  }

  if (success) {
    EchoAI::Instance().OnSuccess(mail_entry->subject);
    player_.Inventory()->AddItem("ETH", 3); // Выдаём награду
    auto* moral = player_.Moral();
    if (moral)
      moral->Add(mail_entry->good ? mail_entry->consequence_good : mail_entry->consequence_bad);
    // Можно добавить публикацию письма в дневнике через diary_
    EchoAI::Instance().DiaryNote();
  } else {
    auto* health = player_.Health();
    if (health) {
      health->Damage(1);
      EchoAI::Instance().OnStatChange("HP", health->Get());
      if (health->Get() <= 0) {
        std::cout << Utils::Color("GAME OVER. Press any key to try over.\n", "red");
        std::string dummy;
        std::getline(std::cin, dummy);
        // Сброс
        *health = HealthComponent(3);
        *player_.Moral() = MoralComponent(0);
        player_.Inventory()->AddItem("ETH", 0);
        EchoAI::Instance().OnPlayerAction("Начинаем сначала!");
      }
    }
  }
}