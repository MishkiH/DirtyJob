#ifndef INVENTORY_COMPONENT_HPP_
#define INVENTORY_COMPONENT_HPP_

#include <map>
#include <string>

#include "Component.hpp"

class InventoryComponent : public Component {
 public:
  InventoryComponent() = default;
  ~InventoryComponent() override = default;

  std::string GetID() const override { return "InventoryComponent"; }

  void AddItem(const std::string& item_id, int count = 1) {
    items_[item_id] += count;
  }

  bool RemoveItem(const std::string& item_id, int count = 1) {
    auto it = items_.find(item_id);
    if (it != items_.end() && it->second >= count) {
      it->second -= count;
      if (it->second == 0) items_.erase(it);
      return true;
    }
    return false;
  }

  int GetCount(const std::string& item_id) const {
    auto it = items_.find(item_id);
    return (it != items_.end()) ? it->second : 0;
  }

  const std::map<std::string, int>& GetAllItems() const { return items_; }

 private:
  std::map<std::string, int> items_;
};

#endif  // INVENTORY_COMPONENT_HPP_
