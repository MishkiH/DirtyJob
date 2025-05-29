#include "../include/Shop.hpp"
#include "../include/Console.hpp"
#include "../include/components/HasInventory.hpp"
#include "../include/components/HasStats.hpp"
#include <iostream>
#include <algorithm>

Shop::Shop() {
    initializeItems();
}

void Shop::initializeItems() {
    //прокачка
    items.emplace_back("AI-Помощник Echo", 
        "Умный ассистент, предупреждает об опасностях во время взлома", 
        "Upgrade", 15, "permanent");
        
    items.emplace_back("Улучшенное железо", 
        "Быстрые процессоры сокращают время взлома с 4 до 2 фаз", 
        "Upgrade", 25, "permanent");
    
    //расходники
    items.emplace_back("GhostKey", 
        "Универсальный ключ, пропускает 1-2 фазы взлома", 
        "Consumable", 8, "consumable");
        
    items.emplace_back("Kernel Reset v2", 
        "Восстанавливает здоровье на 30-50 единиц", 
        "Consumable", 12, "consumable");
        
    items.emplace_back("Stealth Module", 
        "Снижает риск обнаружения во время операций", 
        "Consumable", 10, "consumable");
}

void Shop::showInterface() {
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                          D-SHOP                              ║\n", Console::CYAN);
    std::cout << Console::colorize("║                   Магазин цифровых инструментов               ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    
    displayItems();
    
    std::cout << "\nДоступные команды:\n";
    std::cout << Console::colorize("/buy {название}", Console::GREEN) << " - купить предмет\n";
    std::cout << Console::colorize("/inventory", Console::GREEN) << " - показать инвентарь\n";
    std::cout << Console::colorize("/switch main", Console::GREEN) << " - вернуться в главное меню\n";
}

void Shop::displayItems() const {
    std::cout << "\n" << Console::colorize("🛒 ТОВАРЫ В НАЛИЧИИ:", Console::YELLOW) << "\n\n";
    
    std::cout << Console::colorize("📈 УЛУЧШЕНИЯ ПЕРСОНАЖА:", Console::MAGENTA) << "\n";
    for (const auto& item : items) {
        if (item.category == "Upgrade") {
            std::cout << Console::colorize("  • ", Console::GREEN) << Console::colorize(item.name, Console::WHITE);
            std::cout << Console::colorize(" - " + std::to_string(item.price) + " ETH", Console::YELLOW) << "\n";
            std::cout << Console::colorize("    " + item.description, Console::GRAY) << "\n\n";
        }
    }
    
    std::cout << Console::colorize("⚡ РАСХОДНЫЕ МАТЕРИАЛЫ:", Console::BLUE) << "\n";
    for (const auto& item : items) {
        if (item.category == "Consumable") {
            std::cout << Console::colorize("  • ", Console::GREEN) << Console::colorize(item.name, Console::WHITE);
            std::cout << Console::colorize(" - " + std::to_string(item.price) + " ETH", Console::YELLOW) << "\n";
            std::cout << Console::colorize("    " + item.description, Console::GRAY) << "\n\n";
        }
    }
}

bool Shop::processCommand(const std::string& command, Entity& player) {
    if (command.substr(0, 5) == "/buy ") {
        std::string itemName = command.substr(5);
        return buyItem(itemName, player);
    }
    
    if (command == "/inventory") {
        auto inventory = player.getComponent<HasInventory>();
        if (inventory) {
            inventory->displayInventory();
        } else {
            std::cout << Console::colorize("Ошибка: Инвентарь недоступен\n", Console::RED);
        }
        return true;
    }
    
    return false;
}

bool Shop::buyItem(const std::string& itemName, Entity& player) {
    //находим товар
    auto it = std::find_if(items.begin(), items.end(),
        [&itemName](const ShopItem& item) { return item.name == itemName; });
    
    if (it == items.end()) {
        std::cout << Console::colorize("Товар не найден. Проверьте название.\n", Console::RED);
        return false;
    }
    
    const auto& item = *it;
    
    auto inventory = player.getComponent<HasInventory>();
    if (!inventory) {
        std::cout << Console::colorize("Ошибка: Инвентарь недоступен\n", Console::RED);
        return false;
    }
    
    // Проверяем наличие денег
    if (!inventory->spendEthereum(item.price)) {
        std::cout << Console::colorize("Недостаточно средств! Нужно " + std::to_string(item.price) + " ETH\n", Console::RED);
        std::cout << Console::colorize("У вас: " + std::to_string(inventory->getEthereum()) + " ETH\n", Console::YELLOW);
        return false;
    }
    
    //добавляем в инвентарь
    Item newItem(item.name, item.description, item.type);
    inventory->addItem(newItem);
    
    std::cout << Console::colorize("═══ ПОКУПКА СОВЕРШЕНА ═══\n", Console::GREEN);
    std::cout << Console::colorize("✅ Куплено: ", Console::GREEN) << Console::colorize(item.name, Console::CYAN) << "\n";
    std::cout << Console::colorize("💰 Потрачено: ", Console::YELLOW) << item.price << " ETH\n";
    std::cout << Console::colorize("💳 Остаток: ", Console::YELLOW) << inventory->getEthereum() << " ETH\n";
    
    // Особые эффекты для некоторых предметов
    if (item.name == "AI-Помощник Echo") {
        std::cout << Console::colorize("\n🤖 ECHO: \"Система активирована. Готов к работе!\"\n", Console::BLUE);
    } else if (item.name == "Kernel Reset v2") {
        auto stats = player.getComponent<HasStats>();
        if (stats) {
            int healAmount = 35;
            stats->restoreHealth(healAmount);
            std::cout << Console::colorize("\n💚 Здоровье восстановлено на " + std::to_string(healAmount) + " единиц\n", Console::GREEN);
        }
    }
    
    return true;
}