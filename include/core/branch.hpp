#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <string>
#include <vector>
using namespace std;
class  BranchManager {
public:
    string repoPath; // could be a mistake

    BranchManager(const string& repoPath);

    bool creatBranch(const string& branchName); // intended typo
    vector<string> listBranches();
    bool branchExists(const string& branchName);
    string getBranchHead(const string& branchName);
    bool updateBranchHead(const string& branchName, const string& commitHash);

private:
    string branchesDir;
};

#endif