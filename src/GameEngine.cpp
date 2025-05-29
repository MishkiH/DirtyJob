#include "../include/GameEngine.hpp"
#include "../include/components/HasInventory.hpp"
#include "../include/components/HasStats.hpp"
#include <iostream>
#include <sstream>

GameEngine::GameEngine() : running(true), currentWorkspace("main") {
    // создание игрока с необходимыми компонентами
    player = std::make_unique<Entity>("Player");
    player->addComponent(std::make_unique<HasInventory>());
    player->addComponent(std::make_unique<HasStats>());
    
    // инициализация системы
    workspaceManager = std::make_unique<WorkspaceManager>();
    mailSystem = std::make_unique<MailSystem>();
    diarySystem = std::make_unique<DiarySystem>();
    shop = std::make_unique<Shop>();
    
    // загрузка начальных данные
    mailSystem->loadMails();
    diarySystem->loadDiary();
}

GameEngine::~GameEngine() = default;

void GameEngine::run() {
    showWelcome();
    
    std::string input;
    while (running) {
        showMainScreen();
        std::cout << Console::colorize(">>> ", Console::GREEN);
        std::getline(std::cin, input);
        
        if (!processCommand(input)) {
            std::cout << Console::colorize("Неизвестная команда. Попробуйте /help\n", Console::RED);
        }
        
        std::cout << "\n";
    }
}

void GameEngine::showWelcome() {
    Console::clear();
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                        ГРЯЗНАЯ РАБОТА                        ║\n", Console::CYAN);
    std::cout << Console::colorize("║                    Хакерский симулятор v0.1                  ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    std::cout << "\nДобро пожаловать в мир цифровых теней...\n\n";
}

void GameEngine::showMainScreen() {
    std::cout << Console::colorize("═══════════════════════════════════════════════════════════════\n", Console::BLUE);
    std::cout << Console::colorize("РАБОЧЕЕ ОКРУЖЕНИЕ ХАКЕРА", Console::YELLOW) << " [" << currentWorkspace << "]\n";
    std::cout << Console::colorize("═══════════════════════════════════════════════════════════════\n", Console::BLUE);
    std::cout << "\nДоступные команды:\n";
    std::cout << Console::colorize("/switch_ls", Console::GREEN) << " - список рабочих пространств\n";
    std::cout << Console::colorize("/switch {id}", Console::GREEN) << " - переключить пространство\n";
    std::cout << Console::colorize("/mystats", Console::GREEN) << " - показать характеристики\n";
    std::cout << Console::colorize("/help", Console::GREEN) << " - справка\n";
    std::cout << Console::colorize("/quit", Console::GREEN) << " - выход\n";
}

bool GameEngine::processCommand(const std::string& command) {
    if (command.empty()) return false;
    
    if (command == "/quit" || command == "/exit") {
        running = false;
        std::cout << Console::colorize("Соединение разорвано...\n", Console::RED);
        return true;
    }
    
    if (command == "/help") {
        std::cout << Console::colorize("═══ СПРАВКА ═══\n", Console::YELLOW);
        std::cout << "Навигация по системе:\n";
        std::cout << "/switch_ls - показать доступные пространства\n";
        std::cout << "/switch mail - почтовый клиент (задания)\n";
        std::cout << "/switch diary - дневник (история)\n";
        std::cout << "/switch shop - магазин предметов\n";
        std::cout << "/mystats - ваши характеристики\n";
        return true;
    }
    
    if (command == "/switch_ls") {
        workspaceManager->listWorkspaces();
        return true;
    }
    
    if (command.substr(0, 8) == "/switch ") {
        processSwitch(command);
        return true;
    }
    
    if (command == "/mystats") {
        showStats();
        return true;
    }
    
    // команды текущего пространства
    if (currentWorkspace == "mail") {
        return mailSystem->processCommand(command, *player);
    } else if (currentWorkspace == "diary") {
        return diarySystem->processCommand(command, *player);
    } else if (currentWorkspace == "shop") {
        return shop->processCommand(command, *player);
    }
    
    return false;
}

void GameEngine::processSwitch(const std::string& command) {
    std::string workspace = command.substr(8);
    
    if (workspaceManager->isValidWorkspace(workspace)) {
        currentWorkspace = workspace;
        Console::clear();
        
        if (workspace == "mail") {
            mailSystem->showInterface();
        } else if (workspace == "diary") {
            diarySystem->showInterface();
        } else if (workspace == "shop") {
            shop->showInterface();
        } else {
            showMainScreen();
        }
    } else {
        std::cout << Console::colorize("Ошибка: Неизвестное пространство '" + workspace + "'\n", Console::RED);
    }
}

void GameEngine::showStats() {
    auto stats = player->getComponent<HasStats>();
    if (stats) {
        stats->displayStats();
    } else {
        std::cout << Console::colorize("Ошибка: Статистика недоступна\n", Console::RED);
    }
}