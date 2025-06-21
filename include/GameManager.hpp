#ifndef GAME_MANAGER_HPP_
#define GAME_MANAGER_HPP_

#include <string>
#include <map>
#include <memory>
#include "Player.hpp"
#include "EchoAI.hpp"
#include "Shop.hpp"
#include "Mail.hpp"
#include "Diary.hpp"
#include "MiniGame.hpp"
#include "MiniGames_FWByPass.hpp"
#include "MiniGames_BruteForcer.hpp"
#include "MiniGames_ProtocolSimon.hpp"

class GameManager {
 public:
  GameManager();

  void Run();

 private:
  // Рабочие пространства
  enum class Workspace { Mail, Diary, Shop, Main };
  std::map<std::string, Workspace> workspace_map_;
  Workspace current_ws_;
  std::string current_ws_id_;
  std::map<Workspace, std::string> ws_ids_;

  // Игровые сущности и сервисы
  Player player_;
  Shop shop_;
  Mail mail_;
  Diary diary_;

  // Новые поля для выбора писем
  int selected_mail_id_ = -1;
  bool awaiting_accept_reject_ = false;

  // Кэш мини-игр (для возможного расширения)
  std::unique_ptr<MiniGame> current_mini_game_;

  // Генерация случайных ID рабочих пространств
  void GenerateWorkspaceIds();

  // Командный интерфейс
  void ShowPrompt() const;
  void HandleCommand(const std::string& cmd);

  // Отображение экранов
  void ShowMail();
  void ShowDiary();
  void ShowShop();
  void ShowMain();
  void ShowStats();

  // Обработка заказов, запуск мини-игр, и применение их результатов
  void StartMissionByID(int mail_id);

  // Служебные
  void ListWorkspaces() const;
  Workspace WorkspaceById(const std::string& id) const;
};

#endif  // GAME_MANAGER_HPP_
