#include <iostream>
#include <vector>
#include "../src/commands/add.hpp"

using namespace std;

void handleBranchCommand(const string& repoPath, const vector<string>& args);
void handleCheckoutCommand(const string& repoPath, const vector<string>& args);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cerr << "Usage: minigit <command> [args]\n";
        return 1;
    }
    
    string command = argv[1];

    string repoPath = ".minigit/";
    vector<string> args(argv + 1, argv + argc);

    if (command == "add"){
        if (argc < 3){
            cerr << "Usage: minigit add <filename>\n";
            return 1;
        }
        string filename = argv[2];
        addFile(filename);
        
    } else if (command == "branch") {
        handleBranchCommand(repoPath, args);
        
    } else if (command == "checkout") {
        handleCheckoutCommand(repoPath, args);
        
    } else
    {
        cerr << "Unknown command.\n";
    }

    return 0;
}
