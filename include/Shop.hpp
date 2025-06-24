#ifndef SHOP_HPP_
#define SHOP_HPP_

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Player.hpp"

struct ShopItem {
  std::string id;
  std::string name;
  double price;
  std::string desc;
};

class Shop {
 public:
  Shop(const std::string& data_file);

  void Show() const;
  bool Buy(Player& player, const std::string& item_id);
  const ShopItem* FindItem(const std::string& item_id) const;
  void ApplyItem(Player& player, const std::string& item_id);

 private:
  std::vector<ShopItem> items_;
  void LoadFromFile(const std::string& data_file);
};

#endif  // SHOP_HPP_
