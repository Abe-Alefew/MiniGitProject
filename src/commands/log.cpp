#include "core/repository.hpp"
#include <iostream>
using namespace std;
using namespace mgit;
int logCommits()
{
    Repository repo;

    if (!repo.isInitialized())
    {
        cout << "Repository not initialized.\n" << endl;
        return 1;
    }

    Commit current = repo.getHeadCommit();

   while (!current.hash.empty())
{
    cout << "commit: " << current.hash << "\n";
    cout << "Date: " << current.timestamp << "\n";
    cout << "Message: " << current.message << "\n";
    cout << "Files:\n";
    for (const auto& [filename, blob] : current.files) {
        cout << "  " << filename << "\n";
    }
    cout << "\n";

    if (current.parent.empty()) break; 

    current = repo.getCommitById(current.parent);
}

    return 0;
}
