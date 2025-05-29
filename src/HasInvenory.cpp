#include "../../include/components/HasInventory.hpp"
#include "../../include/Console.hpp"
#include <iostream>
#include <algorithm>

HasInventory::HasInventory() : ethereum(10) {
    // Начальные предметы можно добавить здесь если нужно
}

void HasInventory::addItem(const Item& item) {
    auto it = std::find_if(items.begin(), items.end(),
        [&item](const Item& existing) { return existing.name == item.name; });
    
    if (it != items.end()) {
        it->quantity += item.quantity;
    } else {
        items.push_back(item);
    }
}

bool HasInventory::removeItem(const std::string& itemName, int quantity) {
    auto it = std::find_if(items.begin(), items.end(),
        [&itemName](const Item& item) { return item.name == itemName; });
    
    if (it != items.end() && it->quantity >= quantity) {
        it->quantity -= quantity;
        if (it->quantity <= 0) {
            items.erase(it);
        }
        return true;
    }
    return false;
}

bool HasInventory::hasItem(const std::string& itemName) const {
    return std::find_if(items.begin(), items.end(),
        [&itemName](const Item& item) { return item.name == itemName; }) != items.end();
}

Item* HasInventory::getItem(const std::string& itemName) {
    auto it = std::find_if(items.begin(), items.end(),
        [&itemName](Item& item) { return item.name == itemName; });
    
    return (it != items.end()) ? &(*it) : nullptr;
}

void HasInventory::displayInventory() const {
    std::cout << Console::colorize("╔═══════════════════════════════════════════════════════════════╗\n", Console::CYAN);
    std::cout << Console::colorize("║                           ИНВЕНТАРЬ                          ║\n", Console::CYAN);
    std::cout << Console::colorize("╚═══════════════════════════════════════════════════════════════╝\n", Console::CYAN);
    
    std::cout << Console::colorize("💰 Ethereum: ", Console::YELLOW) << ethereum << " ETH\n\n";
    
    if (items.empty()) {
        std::cout << Console::colorize("Инвентарь пуст.\n", Console::GRAY);
        return;
    }
    
    std::cout << Console::colorize("Предметы:\n", Console::WHITE);
    for (const auto& item : items) {
        std::cout << Console::colorize("  • ", Console::GREEN) << item.name;
        if (item.quantity > 1) {
            std::cout << Console::colorize(" x" + std::to_string(item.quantity), Console::YELLOW);
        }
        std::cout << "\n    " << Console::colorize(item.description, Console::GRAY) << "\n";
    }
}

bool HasInventory::spendEthereum(int amount) {
    if (ethereum >= amount) {
        ethereum -= amount;
        return true;
    }
    return false;
}