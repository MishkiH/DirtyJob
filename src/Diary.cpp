#include "Diary.hpp"

using json = nlohmann::json;

Diary::Diary(const std::string& data_file) { LoadFromFile(data_file); }

void Diary::LoadFromFile(const std::string& data_file) {
  std::ifstream file(data_file);
  if (!file) return;
  json j;
  file >> j;
  for (const auto& entry : j) {
    DiaryEntry e;
    e.id = entry.value("id", 0);
    e.title = entry.value("title", "");
    e.text = entry.value("text", "");
    e.moral_effect = entry.value("moral_effect", 0);
    e.mission_id = entry["mission_id"].get<int>();
    entries_.push_back(e);
  }
}

void Diary::Show() const {
  std::cout << Utils::Color("=== Дневник ===\n", "magenta");
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

const DiaryEntry* Diary::GetEntryByMissionId(int mission_id) const {
  for (const auto& entry : entries_) {
    if (entry.mission_id && *entry.mission_id == mission_id) return &entry;
  }
  return nullptr;
}

const DiaryEntry* Diary::FindById(int id) const {
  for (const auto& entry : entries_) {
    if (entry.id == id) return &entry;
  }
  return nullptr;
}
DiaryEntry* Diary::FindById(int id) {
  for (auto& entry : entries_) {
    if (entry.id == id) return &entry;
  }
  return nullptr;
}