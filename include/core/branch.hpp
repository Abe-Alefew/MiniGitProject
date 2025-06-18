#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <string>
#include <vector>

class  BranchManager {
public:
    std::string repoPath; // could be a mistake

    BranchManager(const std::string& repoPath);

    bool creatBranch(const std::string& branchName);
    std::vector<std::string> listBranches();
    bool branchExists(const std::string& branchName);
    std::string getBranchHead(const std::string& branchName);
    bool updateBranchHead(const std::string& branchName, const std::string& commitHash);

private:
    std::string branchesDir;
};

#endif

