#ifndef DIARY_HPP_
#define DIARY_HPP_

#include <vector>
#include <string>

struct DiaryEntry {
  int id;
  std::string title;
  std::string text;
  int moral_effect;
  bool published = false;
};

class Diary {
 public:
  Diary(const std::string& data_file);

  void Show() const;
  const DiaryEntry* GetEntryById(int num) const;
  DiaryEntry* Publish(int num);

 private:
  std::vector<DiaryEntry> entries_;
  void LoadFromFile(const std::string& data_file);
};

#endif  // DIARY_HPP_
