#pragma once
#include "../Component.hpp"
#include <vector>
#include <string>

struct Item {
    std::string name;
    std::string description;
    std::string type;
    int quantity;
    
    Item(const std::string& n, const std::string& desc, const std::string& t, int q = 1)
        : name(n), description(desc), type(t), quantity(q) {}
};

class HasInventory : public Component {
public:
    HasInventory();
    ~HasInventory() override = default;
    
    void addItem(const Item& item);
    bool removeItem(const std::string& itemName, int quantity = 1);
    bool hasItem(const std::string& itemName) const;
    Item* getItem(const std::string& itemName);
    
    void displayInventory() const;
    const std::vector<Item>& getItems() const { return items; }
    
    int getEthereum() const { return ethereum; }
    void setEthereum(int amount) { ethereum = amount; }
    void addEthereum(int amount) { ethereum += amount; }
    bool spendEthereum(int amount);
    
private:
    std::vector<Item> items;
    int ethereum;
};

//пасхалка