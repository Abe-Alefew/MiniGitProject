#include "../../include/core/branch.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

void handleCheckoutCommand(const string& repoPath, const vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: \n";
        cout << "checkout <branch>\n";
        return;
    }
    string branchName = args[1];
    BranchManager bm(repoPath);
    if (!bm.branchExists(branchName)) {
        cout << "ERROR: Branch '" << branchName << "' does not  exist.\n";
        return;
    }
    //get HEAD commit of target branch
    string targetCommit = bm.getBranchHead(branchName);
    //update .minigit/HEAD to point to this branch
    ofstream headFile(repoPath + "HEAD");

    if (headFile) {
        headFile << targetCommit;
        headFile.close();
    } else {
        cout << "Error: could not update HEAD.\n";
        return;
    }
    cout << "switched to Branch '" << branchName << "'.\n";
}