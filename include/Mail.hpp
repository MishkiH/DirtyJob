#ifndef MAIL_HPP_
#define MAIL_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Player.hpp"

struct MailEntry {
  int id;
  std::string subject;
  std::string sender;
  std::string content;
  bool good;  // гуманное задание
  int consequence_good;
  int consequence_bad;
  int reward = 0;
  bool taken = false;
  bool finished = false;
  bool rejected = false;
};

class Mail {
 public:
  Mail(const std::string& data_file);

  void ShowInbox() const;
  MailEntry* GetMailByID(int num);
  void RejectMail(int id);
  MailEntry* Take(int num);  // берём задание, помечаем taken
  void RejectOtherMailsInCurrentWave(int except_id);

  int Count() const { return static_cast<int>(mails_.size()); }
  int CurrentWave() const;
  std::vector<const MailEntry*> GetAvailableMails() const;

 private:
  std::vector<MailEntry> mails_;
  void LoadFromFile(const std::string& data_file);
};

#endif  // MAIL_HPP_
