#ifndef GAME_MANAGER_HPP_
#define GAME_MANAGER_HPP_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>

#include "Diary.hpp"
#include "EchoAI.hpp"
#include "Mail.hpp"
#include "MiniGame.hpp"
#include "MiniGames_BruteForcer.hpp"
#include "MiniGames_FWByPass.hpp"
#include "MiniGames_ProtocolSimon.hpp"
#include "Player.hpp"
#include "Shop.hpp"
#include "Utils.hpp"

class GameManager {
 public:
  GameManager();

  void Run();

 private:
  // рабочие пространства
  enum class Workspace { Mail, Diary, Shop, Main };
  std::map<std::string, Workspace> workspace_map_;
  Workspace current_ws_;
  std::string current_ws_id_;
  std::map<Workspace, std::string> ws_ids_;

  // игровые сущности и сервисы
  Player player_;
  Shop shop_;
  Mail mail_;
  Diary diary_;

  // новые поля для выбора писем
  int selected_mail_id_ = -1;
  bool awaiting_accept_reject_ = false;

  // кэш мини-игр (для возможного расширения)
  std::unique_ptr<MiniGame> current_mini_game_;

  // генерация случайных ID рабочих пространств
  void GenerateWorkspaceIds();

  // командный интерфейс
  void ShowPrompt() const;
  void HandleCommand(const std::string& cmd);

  // отображение экранов
  void ShowMail();
  void ShowDiary();
  void ShowShop();
  void ShowMain();
  void ShowStats();

  // обработка заказов, запуск мини-игр, и применение их результатов
  void StartMissionByID(int mail_id);
  void ShowDiaryEntry(const DiaryEntry& entry);
  void ShowDiaryEntryAndEnd(const DiaryEntry& entry);
  void ShowMail5Choice();
  bool awaiting_mail5_choice_ = false;

  // служебные
  void ListWorkspaces() const;
  Workspace WorkspaceById(const std::string& id) const;
};

#endif  // GAME_MANAGER_HPP_
