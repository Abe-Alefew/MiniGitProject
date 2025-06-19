#include <iostream>
#include <string>
#include <vector>
using namespace std;

// intializing the functions
int initCommand();
int addCommand(const string& filename);
int commitCommand(const string& message);
int logCommits();
void handleBranchCommand(const string& repoPath, const vector<string>& args);
void handleCheckoutCommand(const string& repoPath, const vector<string>& args);
void mergeBranch(const string& targetBranch);  // <-- Add prototype here

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: ./minigit <command>\n";
        return 1;
    }

    const vector<string> args(argv + 1, argv + argc);
    const string repoPath = ".minigit/";
    string command = argv[1];

    if (command == "init")
        return initCommand();
    else if (command == "add") {
        if(argc < 3) {
            cout << "Usage: ./minigit add <file>\n" << endl;
            return 1;
        }
        return addCommand(argv[2]);
    } 
    else if(command == "commit") {
        string message;
        if(argc >= 4 && string(argv[2]) == "-m") {
            message = argv[3];
        } else {
            cout << "Usage: ./minigit commit -m \"message\"\n";
            return 1;
        }
        return commitCommand(message);
    } 
    else if (command == "log") {
        return logCommits();
    } 
    else if (command == "branch") {
        handleBranchCommand(repoPath, args);
    } 
    else if (command == "checkout") {
        handleCheckoutCommand(repoPath, args);
    }
    else if (command == "merge") {
        if(argc < 3) {
            cout << "Usage: ./minigit merge <branch-name>\n";
            return 1;
        }
        string targetBranch = argv[2];
        mergeBranch(targetBranch);
        cout << "Merged branch '" << targetBranch << "' into current branch.\n";
    }
    else {
        cout << "Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}
