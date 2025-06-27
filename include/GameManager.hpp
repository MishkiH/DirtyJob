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

  // состояния выбора
  int selected_mail_id_ = -1;
  bool awaiting_accept_reject_ = false;
  bool awaiting_mail5_choice_ = false;

  // кэш мини-игр
  std::unique_ptr<MiniGame> current_mini_game_;

  // генерация ID рабочих пространств
  void GenerateWorkspaceIds();

  // командный интерфейс
  void ShowPrompt() const;
  void HandleCommand(const std::string& cmd);
  
  // обработчики команд
  void HandleSwitchCommand(std::istringstream& iss);
  void HandleSelectCommand(std::istringstream& iss);
  void HandleMail5Choice(const std::string& choice);
  void HandleAcceptReject(const std::string& cmd);
  void HandleUseCommand(std::istringstream& iss);
  void HandleBuyCommand(std::istringstream& iss);

  // переключение рабочих пространств
  void SwitchToWorkspace(Workspace ws);

  // отображение экранов
  void ShowMail();
  void ShowDiary();
  void ShowShop();
  void ShowMain();
  void ShowStats();  // Добавлен const
  void ListWorkspaces() const;  // Добавлен const

  // работа с почтой
  void ShowMailEntry(int id);
  void HandleRegularMail();
  void HandleSpecialMail();
  void ShowMail5Choice();

  // работа с дневником
  void ShowDiaryEntry(int id, bool show_prompt);
  void ShowDiaryEntryAndEnd(int id);

  // игровая логика
  void StartMissionByID(int mail_id);
  void StartBadEndGame();
  void HandleMissionFailure();
  void GameOver();
};

#endif  // GAME_MANAGER_HPP_