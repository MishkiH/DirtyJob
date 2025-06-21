#include "Mail.hpp"
#include "../extern/nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Mail::Mail(const std::string& data_file) {
  LoadFromFile(data_file);
}

void Mail::LoadFromFile(const std::string& data_file) {
  std::ifstream file(data_file);
  if (!file) {
    std::cerr << "[DEBUG] Не удалось открыть файл: " << data_file << std::endl;
    return; }
  json j;
  file >> j;
  for (const auto& entry : j) {
    mails_.push_back({
      entry.value("id", 0),
      entry.value("subject", ""),
      entry.value("sender", "anon"),
      entry.value("content", ""),
      entry.value("good", false),
      entry.value("consequence_good", 0),
      entry.value("consequence_bad", 0)
    });
  }
}

void Mail::ShowInbox() const {
  std::cout << "\033[33m=== Входящие ===\033[0m\n";
  int num = 1;  for (const auto& mail : mails_) {
    if (!mail.taken && !mail.finished && !mail.rejected)
      std::cout << "Письмо №" << mail.id << ". Тема: " << mail.subject << "\n";
  }
}

MailEntry* Mail::GetMailByID(int id) {
  for (auto& mail : mails_) {
    if (mail.id == id && !mail.taken && !mail.finished && !mail.rejected)
      return &mail;
  }
  return nullptr;
}

void Mail::RejectMail(int id) {
  for (auto& mail : mails_) {
    if (mail.id == id) {
      mail.rejected = true;
      return;
    }
  }
}

MailEntry* Mail::Take(int num) {
  int cnt = 0;
  for (auto& mail : mails_) {
    if (!mail.taken && !mail.finished) {
      ++cnt;
      if (cnt == num) {
        mail.taken = true;
        return &mail;
      }
    }
  }
  return nullptr;
}
