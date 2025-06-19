#include "../../include/core/branch.hpp"
#include <iostream>
using namespace std;

// gets args from main
void handleBranchCommand(const string& repoPath, const vector<string>& args) {
    BranchManager bm(repoPath);
    if (args.size() == 1) { //no argument, means list branches
        auto branches = bm.listBranches();
        cout << "Branches:\n";
        for (const auto& b : branches) {
            cout << "  " << b << endl;
        }
    } else if (args.size() == 2) { // create new branch
        string branchName = args[1];
        if (bm.creatBranch(branchName)) {
            cout << "Branch '" << branchName << "' created successfully.\n";
        } else {
            cout << "ERROR: Branch '" << branchName << "' already exists.\n";
        }
    } else { // there are too many arguements
        cout << "Usage: \n";
        cout << "branch                       #list branches\n";
        cout << "branch <name>                #create new branch\n";
    }
}