#include "../../include/core/branch.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

BranchManager::BranchManager(const std::string &repoPath) {
    this->repoPath = repoPath;
    branchesDir = repoPath + "branches/";
    if (!fs::exists(branchesDir)) {
        fs::create_directories(branchesDir);
    }
}

bool BranchManager::creatBranch(const std::string &branchName) {
    string branchPath = branchesDir + branchName;
    if (fs::exists(branchPath)) {
        return false; //branch already exists
    }
    ifstream headFile(repoPath + "HEAD"); // NEEDS SOME MODIFICATION FOR reopPath!!
    string headCommit;
    if (headFile) {
        getline(headFile, headCommit);
    }
    headFile.close();

    // write HEAD commit hash to new branch file
    ofstream branchFile(branchPath);
    if (branchFile) {
        branchFile << headCommit;
        branchFile.close();
        return true;
    }
    return false;
}

std::vector<std::string> BranchManager::listBranches() {
    vector<string> branches;
    for (const auto& entry : fs::directory_iterator(branchesDir)) {
        branches.push_back(entry.path().filename().string());
    }
    return branches;
}

bool BranchManager::branchExists(const std::string &branchName) {
    return fs::exists(branchesDir + branchName);
}

std::string BranchManager::getBranchHead(const std::string &branchName) {
    ifstream branchFile(branchesDir + branchName);
    string commitHash;
    if (branchFile) {
        getline(branchFile, commitHash);
    }
    return commitHash;
}

bool BranchManager::updateBranchHead(const std::string &branchName, const std::string &commitHash) {
    if (!branchExists(branchName)) return false;
    ofstream branchFile(branchesDir + branchName);
    if (branchFile) {
        branchFile << commitHash;
        return true;
    }
    return false;
}





