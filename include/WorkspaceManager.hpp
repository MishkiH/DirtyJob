#pragma once
#include <vector>
#include <string>

struct Workspace {
    std::string id;
    std::string name;
    std::string description;
    
    Workspace(const std::string& i, const std::string& n, const std::string& desc)
        : id(i), name(n), description(desc) {}
};

class WorkspaceManager {
public:
    WorkspaceManager();
    ~WorkspaceManager() = default;
    
    void listWorkspaces() const;
    bool isValidWorkspace(const std::string& id) const;
    const Workspace* getWorkspace(const std::string& id) const;
    
private:
    std::vector<Workspace> workspaces;
    void initializeWorkspaces();
};