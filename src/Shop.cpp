#include "Shop.hpp"

using json = nlohmann::json;

Shop::Shop(const std::string& data_file) { LoadFromFile(data_file); }

void Shop::LoadFromFile(const std::string& data_file) {
  std::ifstream file(data_file);
  if (!file) return;
  json j;
  file >> j;
  for (const auto& item : j) {
    items_.push_back({item.value("id", ""), item.value("name", ""),
                      item.value("price", 0.0), item.value("desc", "")});
  }
}

void Shop::Show() const {
  std::cout << EchoAI::EchoStyle("[Echo][D-Shop]\n");
  for (const auto& item : items_) {
    std::cout << "ID: " << item.id << " | " << item.name << " | " << item.price
              << " ETH\n"
              << "  " << item.desc << "\n";
  }
}

const ShopItem* Shop::FindItem(const std::string& item_id) const {
  for (const auto& item : items_) {
    if (item.id == item_id) return &item;
  }
  return nullptr;
}

bool Shop::Buy(Player& player, const std::string& item_id) {
  auto* item = FindItem(item_id);
  if (!item) {
    EchoAI::Instance().OnFail("Такого товара нет");
    return false;
  }

  int eth_count = player.Inventory()->GetCount("ETH");
  if (eth_count < item->price) {
    EchoAI::Instance().OnFail("Маловато ETH, понимаешь ли.");
    return false;
  }

  player.Inventory()->RemoveItem("ETH", static_cast<int>(item->price));
  player.Inventory()->AddItem(item->id, 1);

  EchoAI::Instance().OnBuy(item->name);

  if (item->id == "kernel_v2") {
    EchoAI::Instance().RemindCommand("/use kernel_v2");
  } else if (item->id == "coffee") {
    EchoAI::Instance().RemindCommand("/use coffee");
  }

  return true;
}

void Shop::ApplyItem(Player& player, const std::string& item_id) {
  if (player.Inventory()->GetCount(item_id) == 0) {
    EchoAI::Instance().OnFail("У тебя нет этого предмета");
    return;
  }

  if (item_id == "echo_upgrade") {
    EchoAI::Instance().OnSuccess(
        "Echo теперь с тобой до конца. Ты не рад? А мне пофиг.");
    player.Inventory()->RemoveItem(item_id);
    // Флаг прокачки Echo можно реализовать в отдельном компоненте/флаге
  } else if (item_id == "kernel_v2") {
    auto* health = player.Health();
    if (health) {
      health->UpgradeMax(1);
      EchoAI::Instance().OnStatChange("MaxHP", health->Max());
    }
    player.Inventory()->RemoveItem(item_id);
  } else if (item_id == "coffee") {  // TODO: Сделать грёбаный кофе)
    EchoAI::Instance().OnSuccess(
        "Coffee.exe принят. Станешь внимательнее (на время).");
    player.Inventory()->RemoveItem(item_id);
    // Coffee активируется на следующей миссии
  } else {
    EchoAI::Instance().OnFail("Этот предмет не предмет.");
  }
}
