#include "../include/WorkspaceManager.hpp"
#include "../include/Console.hpp"
#include <iostream>
#include <algorithm>

WorkspaceManager::WorkspaceManager() {
    initializeWorkspaces();
}

void WorkspaceManager::initializeWorkspaces() {
    workspaces.emplace_back("main", "Главная", "Основное рабочее пространство");
    workspaces.emplace_back("mail", "Почта", "Входящие письма с заданиями");
    workspaces.emplace_back("diary", "Дневник", "Личные записи и размышления");
    workspaces.emplace_back("shop", "D-Shop", "Магазин цифровых инструментов");
}

void WorkspaceManager::listWorkspaces() const {
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                    РАБОЧИЕ ПРОСТРАНСТВА                      ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    
    for (const auto& workspace : workspaces) {
        std::cout << Console::colorize("  [" + workspace.id + "] ", Console::GREEN);
        std::cout << Console::colorize(workspace.name, Console::YELLOW) << "\n";
        std::cout << Console::colorize("      " + workspace.description, Console::GRAY) << "\n\n";
    }
    
    std::cout << Console::colorize("Используйте /switch {id} для перехода\n", Console::BLUE);
}

bool WorkspaceManager::isValidWorkspace(const std::string& id) const {
    return std::find_if(workspaces.begin(), workspaces.end(),
        [&id](const Workspace& ws) { return ws.id == id; }) != workspaces.end();
}

const Workspace* WorkspaceManager::getWorkspace(const std::string& id) const {
    auto it = std::find_if(workspaces.begin(), workspaces.end(),
        [&id](const Workspace& ws) { return ws.id == id; });
    
    return (it != workspaces.end()) ? &(*it) : nullptr;
}