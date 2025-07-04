#ifndef DIARY_HPP_
#define DIARY_HPP_

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "../extern/nlohmann/json.hpp"
#include "Utils.hpp"

struct DiaryEntry {
  int id;
  std::string title;
  std::string text;
  bool published = false;
  std::optional<int> mission_id = std::nullopt;
  int moral_effect;
};

class Diary {
 public:
  Diary(const std::string& data_file);
  const std::vector<DiaryEntry>& Entries() const { return entries_; }

  void Show() const;
  const DiaryEntry* GetEntryById(int num) const;

  DiaryEntry* FindById(int id);

 private:
  std::vector<DiaryEntry> entries_;
  void LoadFromFile(const std::string& data_file);
};

#endif  // DIARY_HPP_
