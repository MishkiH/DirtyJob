#pragma once
#include "Entity.hpp"
#include "WorkspaceManager.hpp"
#include "MailSystem.hpp"
#include "DiarySystem.hpp"
#include "Shop.hpp"
#include "Console.hpp"
#include <memory>
#include <string>

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    
    void run();
    bool processCommand(const std::string& command);
    
private:
    void showWelcome();
    void showMainScreen();
    void processSwitch(const std::string& command);
    void showStats();
    
    std::unique_ptr<Entity> player;
    std::unique_ptr<WorkspaceManager> workspaceManager;
    std::unique_ptr<MailSystem> mailSystem;
    std::unique_ptr<DiarySystem> diarySystem;
    std::unique_ptr<Shop> shop;
    
    bool running;
    std::string currentWorkspace;
};