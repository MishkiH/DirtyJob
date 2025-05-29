#pragma once
#include "Entity.hpp"
#include "components/HasInventory.hpp"
#include <vector>
#include <string>

struct ShopItem {
    std::string name;
    std::string description;
    std::string category;
    int price;
    std::string type;
    
    ShopItem(const std::string& n, const std::string& desc, const std::string& cat, int p, const std::string& t)
        : name(n), description(desc), category(cat), price(p), type(t) {}
};

class Shop {
public:
    Shop();
    ~Shop() = default;
    
    void showInterface();
    bool processCommand(const std::string& command, Entity& player);
    
private:
    std::vector<ShopItem> items;
    
    void displayItems() const;
    bool buyItem(const std::string& itemName, Entity& player);
    void initializeItems();
};