#pragma once
#include "Entity.hpp"
#include <vector>
#include <string>

struct Mail {
    int id;
    std::string subject;
    std::string content;
    std::string sender;
    bool isRead;
    int reward;
    
    Mail(int i, const std::string& s, const std::string& c, const std::string& sender, int r)
        : id(i), subject(s), content(c), sender(sender), isRead(false), reward(r) {}
};

class MailSystem {
public:
    MailSystem();
    ~MailSystem() = default;
    
    void loadMails();
    void showInterface();
    bool processCommand(const std::string& command, Entity& player);
    
private:
    std::vector<Mail> mails;
    
    void displayMailList() const;
    void displayMail(int id) const;
    bool takeMission(int id, Entity& player);
    void generateDefaultMails();
};