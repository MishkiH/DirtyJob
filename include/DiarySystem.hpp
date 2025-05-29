#pragma once
#include "Entity.hpp"
#include <vector>
#include <string>

struct DiaryEntry {
    int id;
    std::string title;
    std::string content;
    std::string date;
    bool isPublished;
    
    DiaryEntry(int i, const std::string& t, const std::string& c, const std::string& d)
        : id(i), title(t), content(c), date(d), isPublished(false) {}
};

class DiarySystem {
public:
    DiarySystem();
    ~DiarySystem() = default;
    
    void loadDiary();
    void showInterface();
    bool processCommand(const std::string& command, Entity& player);
    
    void addEntry(const DiaryEntry& entry);
    
private:
    std::vector<DiaryEntry> entries;
    
    void displayEntryList() const;
    void displayEntry(int id) const;
    bool publishEntry(int id, Entity& player);
    void generateDefaultEntries();
};