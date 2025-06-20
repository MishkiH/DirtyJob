#include "Diary.hpp"
#include "../extern/nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Diary::Diary(const std::string& data_file) {
  LoadFromFile(data_file);
}

void Diary::LoadFromFile(const std::string& data_file) {
  std::ifstream file(data_file);
  if (!file) return;
  json j;
  file >> j;
  for (const auto& entry : j) {
    entries_.push_back({
      entry.value("id", 0),
      entry.value("title", ""),
      entry.value("text", ""),
      entry.value("moral_effect", 0)
    });
  }
}

void Diary::Show() const {
  std::cout << "\033[35m=== Дневник ===\033[0m\n";
  int num = 1;
  for (const auto& entry : entries_) {
    if (entry.published)
      std::cout << "Запись №" << num++ << ". " << entry.title << "\n";
  }
}

const DiaryEntry* Diary::GetEntryById(int num) const {
  int cnt = 0;
  for (const auto& entry : entries_) {
    if (entry.published) {
      ++cnt;
      if (cnt == num) return &entry;
    }
  }
  return nullptr;
}

DiaryEntry* Diary::Publish(int num) {
  int cnt = 0;
  for (auto& entry : entries_) {
    if (!entry.published) {
      ++cnt;
      if (cnt == num) {
        entry.published = true;
        return &entry;
      }
    }
  }
  return nullptr;
}
