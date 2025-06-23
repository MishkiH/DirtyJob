#include "GameManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <random>
#include <cstdlib>

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
  Utils::ClearScreen();
  // Показываем вступление из дневника (id == 0)
const DiaryEntry* intro = diary_.FindById(0);
if (intro && !intro->published) {
  Utils::ClearScreen();
    std::cout << "\033[35mЗапись №" << intro->id + 1 << ". Конспект.\n";
    std::cout << "\"" << intro->title << "\"\033[0m\n\n";
    PrintSlowlyByChar(intro->text);
    std::cout << "\nY:[Опубликовать] | N:[Удалить]\n";
    char ans;
    std::cin >> ans;
    if (ans == 'Y' || ans == 'y') {
        DiaryEntry* published = diary_.FindById(0);
        if (published) {
            published->published = true;
            auto* moral = player_.Moral();
            if (moral) moral->Add(published->moral_effect);
            std::cout << "Запись опубликована!\n";
        }
    } else {
        std::cout << "Запись удалена.\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    Utils::ClearScreen();
    ShowMain();
    EchoAI::Instance().PrintWelcome();
}
  bool running = true;
  while (running) {
    ShowPrompt();
    std::string line;
    std::getline(std::cin, line);
    if (line == "/exit") {
      current_ws_ = Workspace::Main;
      current_ws_id_ = ws_ids_[Workspace::Main];
      ShowMain();
      continue;
    }
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
  else if (token == "/mystats") {
    ShowStats();
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

    if (id == 5) {
        awaiting_mail5_choice_ = true;
        Utils::ClearScreen();
        std::cout << "Отправитель: " << mail_entry->sender << '\n';
        std::cout << "Тема: " << mail_entry->subject << '\n';
        std::cout << "===========\n";
        std::cout << mail_entry->content << "\n\n";
        std::cout << "Награда за выполнение: " << mail_entry->reward << " ETH\n\n";
        ShowMail5Choice();
        return;
    } else {
        awaiting_accept_reject_ = true;

        Utils::ClearScreen();
        std::cout << "Отправитель: " << mail_entry->sender << '\n';
        std::cout << "Тема: " << mail_entry->subject << '\n';
        std::cout << "===========\n";
        std::cout << mail_entry->content << "\n\n";
        std::cout << "Награда за выполнение: " << mail_entry->reward << " ETH\n" << '\n';
        std::cout << "\nДля принятия заказа введите y/n. Для отмены введите /cancel\n";
        return;
    }
}
else if (awaiting_mail5_choice_) {
    int choice = 0;
    std::istringstream iss_choice(cmd);
    iss_choice >> choice;

    int moral_lvl = player_.Moral()->GetValue();
    bool can_refuse = (moral_lvl > -10); // -10 и выше можно отказаться
    bool can_accept = (moral_lvl < 10);  // 10 и ниже можно принять

    if ((choice == 1 && can_refuse) || (choice == 2 && can_accept)) {
        awaiting_mail5_choice_ = false;
        if (choice == 1) {
            // GOOD END: дневник 5
            const DiaryEntry* found_entry = diary_.FindById(5);
            if (found_entry) {
                ShowDiaryEntryAndEnd(*found_entry);
            }
            std::exit(0);
        } else if (choice == 2) {
            // BAD END: бесконечная мини-игра и дневник 6
            current_mini_game_ = std::make_unique<ProtocolSimon>();
            bool success = current_mini_game_->Play();
            (void)success; // успех невозможен
            const DiaryEntry* found_entry = diary_.FindById(6);
            if (found_entry) {
                ShowDiaryEntryAndEnd(*found_entry);
            }
            std::exit(0);
        }
    } else {
        std::cout << "Вариант недоступен! Повторите выбор:\n";
        ShowMail5Choice();
    }
    return;
}
else if (awaiting_accept_reject_) {
    if (cmd == "y") {
      awaiting_accept_reject_ = false;
      if (selected_mail_id_ != -1)
        StartMissionByID(selected_mail_id_);
      selected_mail_id_ = -1;
      return;
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
        return;
    } else if (cmd == "/cancel") {
        awaiting_accept_reject_ = false;
        selected_mail_id_ = -1;
        ShowMail();
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
  else if (token == "/buy") {
      std::string item_id;
      iss >> item_id;
      if (item_id.empty()) {
          EchoAI::Instance().OnFail("Укажи ID предмета.");
          return;
      }
      shop_.Buy(player_, item_id);
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
  Utils::ClearScreen();
  std::cout << "Доступные рабочие пространства:\n";
  std::cout << "mail:   " << ws_ids_.at(Workspace::Mail) << "\n";
  std::cout << "diary:  " << ws_ids_.at(Workspace::Diary) << "\n";
  std::cout << "shop:   " << ws_ids_.at(Workspace::Shop) << "\n";
  std::cout << "main:   " << ws_ids_.at(Workspace::Main) << "\n";
  std::cout << "Напоминалочка: /switch {id} - для перемещения.\n";
}

GameManager::Workspace GameManager::WorkspaceById(const std::string& id) const {
  auto it = workspace_map_.find(id);
  if (it != workspace_map_.end()) return it->second;
  return Workspace::Main;
}

void GameManager::ShowMail() {
  Utils::ClearScreen();
  mail_.ShowInbox();
  std::cout << Utils::Color("Доступные команды: /select {id} - выбор конкретного письма, /exit - выход в главное меню\n", "cyan");
}

void GameManager::ShowDiary() {
  Utils::ClearScreen();
  diary_.Show();
  std::cout << Utils::Color("Доступные команды: /select {id} - вспомнить запись, /exit - выход в главное меню\n", "cyan");
}

void GameManager::ShowShop() {
  Utils::ClearScreen();
  shop_.Show();
  std::cout << Utils::Color("Доступные команды: /buy {name} - приобрести предмет, /exit - выход в главное меню\n", "cyan");

}

void GameManager::ShowMain() {
  Utils::ClearScreen();
  std::cout << Utils::Color("[Представьте фото котёнка в корзинке, на фоне растёт зелёный лужок. Это ваши обои.]\n", "green");
  std::cout << Utils::Color("Святая всех святых: твоё рабочее место.\n", "blue");
  std::cout << Utils::Color("Доступные команды: /switch_ls, /switch {id}, /mystats, /exit\n", "blue");
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

    // спецлогика для письма №5

    // обычные письма писем
    bool success = false;
    if (mail_entry->id == 1 || mail_entry->id == 2) {
        current_mini_game_ = std::make_unique<FWByPass>("../MiniGames/fwby_maze.json");
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
        player_.Inventory()->AddItem("ETH", mail_entry->reward);
        auto* moral = player_.Moral();
        if (moral)
            moral->Add(mail_entry->good ? mail_entry->consequence_good : mail_entry->consequence_bad);

        // показываем дневниковую запись для этой миссии (если есть)
        const DiaryEntry* found_entry = diary_.FindById(mail_entry->id);
        if (found_entry && !found_entry->published) {
            ShowDiaryEntry(*found_entry);
        }
        Utils::ClearScreen();
        ShowMain();
    } else { // Проигрыш — потеря здоровья и сброс
        auto* health = player_.Health();
        if (health) {
            health->Damage(1);
            EchoAI::Instance().OnStatChange("HP", health->Get());
            if (health->Get() <= 0) {
                std::cout << Utils::Color("GAME OVER. Press any key to try over.\n", "red");
                std::string dummy;
                std::getline(std::cin, dummy);
                *health = HealthComponent(3);
                *player_.Moral() = MoralComponent(0);
                player_.Inventory()->AddItem("ETH", 0);
                EchoAI::Instance().OnPlayerAction("Начинаем сначала!");
            }
        }
    }
}

void GameManager::ShowMail5Choice() {
    int moral_lvl = player_.Moral()->GetValue();
    bool can_refuse = (moral_lvl > -10);
    bool can_accept = (moral_lvl < 10);

    std::string reason_refuse, reason_accept;
    if (!can_refuse)
        reason_refuse = "Уровень морального диссидента слишком низок для отказа от грязной работы.";
    if (!can_accept)
        reason_accept = "Уровень морального диссидента слишком высок для принятия грязного заказа.";

    std::cout << (can_refuse ? "\033[32m" : "\033[90m") << "[1] Отказаться от грязной работы (хорошая концовка)\033[0m\n";
    if (!can_refuse) std::cout << "\033[90mПричина: " << reason_refuse << "\033[0m\n";
    std::cout << (can_accept ? "\033[31m" : "\033[90m") << "[2] Принять грязный заказ (плохая концовка)\033[0m\n";
    if (!can_accept) std::cout << "\033[90mПричина: " << reason_accept << "\033[0m\n";
    std::cout << "Моральный диссидент: " << moral_lvl << "\n";
    std::cout << "Выберите вариант (1/2): ";
}


//для показа записей и завершения
void GameManager::ShowDiaryEntry(const DiaryEntry& entry) {
    std::cout << "\033[35mЗапись №" << (entry.mission_id.has_value() ? std::to_string(*entry.mission_id) : "?") << ". Конспект.\n";
    std::cout << "\"" << entry.title << "\"\033[0m\n\n";
    PrintSlowlyByChar(entry.text);
    EchoAI::Instance().DiaryNote();
    std::cout << "\nY[Опубликовать] | N[Удалить] : ";
    char ans;
    std::cin >> ans;
    auto* moral = player_.Moral();
    if (ans == 'Y' || ans == 'y') {
        DiaryEntry* pub = diary_.FindById(entry.id);
        if (pub) {
            pub->published = true;
            if (moral) moral->Add(pub->moral_effect);
        }
    } else { if (moral) moral->Add(-entry.moral_effect); }
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
}

void GameManager::ShowDiaryEntryAndEnd(const DiaryEntry& entry) {
    ShowDiaryEntry(entry);
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    std::cout << "\n\n\033[36mTo be continued..\033[0m\n";
    std::cin.ignore();
    std::cin.get();
}
