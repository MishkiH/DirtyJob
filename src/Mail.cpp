#include "Mail.hpp"

using json = nlohmann::json;

Mail::Mail(const std::string& data_file) { LoadFromFile(data_file); }

void Mail::LoadFromFile(const std::string& data_file) {
  std::ifstream file(data_file);
  if (!file) return;
  json j;
  file >> j;
  for (const auto& entry : j) {
    mails_.push_back(
        {entry.value("id", 0), entry.value("subject", ""),
         entry.value("sender", "anon"), entry.value("content", ""),
         entry.value("good", false), entry.value("consequence_good", 0),
         entry.value("consequence_bad", 0), entry.value("reward", 0)});
  }
}

int Mail::CurrentWave() const {
  bool finished_1_2 = false, finished_3_4 = false, finished_5 = false;
  for (const auto& mail : mails_) {
    if ((mail.id == 1 || mail.id == 2) && mail.finished) finished_1_2 = true;
    if ((mail.id == 3 || mail.id == 4) && mail.finished) finished_3_4 = true;
    if (mail.id == 5 && mail.finished) finished_5 = true;
  }
  if (!finished_1_2) return 1;
  if (!finished_3_4) return 2;
  if (!finished_5) return 3;
  return 0;
}

std::vector<const MailEntry*> Mail::GetAvailableMails() const {
  int wave = CurrentWave();
  std::vector<const MailEntry*> result;
  for (const auto& mail : mails_) {
    if (mail.taken || mail.finished || mail.rejected) continue;
    if (wave == 1 && (mail.id == 1 || mail.id == 2))
      result.push_back(&mail);
    else if (wave == 2 && (mail.id == 3 || mail.id == 4))
      result.push_back(&mail);
    else if (wave == 3 && mail.id == 5)
      result.push_back(&mail);
  }
  return result;
}

void Mail::RejectOtherMailsInCurrentWave(int except_id) {
  int wave = 0;
  if (except_id == 1 || except_id == 2)
    wave = 1;
  else if (except_id == 3 || except_id == 4)
    wave = 2;
  else if (except_id == 5)
    wave = 3;

  for (auto& mail : mails_) {
    if (mail.id != except_id && !mail.taken && !mail.finished &&
        !mail.rejected) {
      if ((wave == 1 && (mail.id == 1 || mail.id == 2)) ||
          (wave == 2 && (mail.id == 3 || mail.id == 4)) ||
          (wave == 3 && mail.id == 5)) {
        mail.rejected = true;
      }
    }
  }
}

void Mail::ShowInbox() const {
  std::cout << Utils::Color("=== Входящие ===\n", "yellow");
  auto mails = GetAvailableMails();
  for (const auto* mail : mails) {
    std::cout << "Письмо №" << mail->id << ". Тема: " << mail->subject << "\n";
  }
}

MailEntry* Mail::GetMailByID(int id) {
  int wave = CurrentWave();
  for (auto& mail : mails_) {
    if ((mail.id == id) && !mail.taken && !mail.finished && !mail.rejected) {
      if ((wave == 1 && (id == 1 || id == 2)) ||
          (wave == 2 && (id == 3 || id == 4)) || (wave == 3 && id == 5)) {
        return &mail;
      }
    }
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
