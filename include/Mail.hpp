#ifndef MAIL_HPP_
#define MAIL_HPP_

#include <vector>
#include <string>
#include "Player.hpp"

struct MailEntry {
  int id;
  std::string subject;
  std::string description;
  bool good;        // Гуманное задание
  int consequence_good;
  int consequence_bad;
  bool taken = false;
  bool finished = false;
};

class Mail {
 public:
  Mail(const std::string& data_file);

  void ShowInbox() const;
  const MailEntry* GetMailById(int num) const;
  MailEntry* Take(int num); // Берём задание, помечаем taken

  int Count() const { return static_cast<int>(mails_.size()); }

 private:
  std::vector<MailEntry> mails_;
  void LoadFromFile(const std::string& data_file);
};

#endif  // MAIL_HPP_
