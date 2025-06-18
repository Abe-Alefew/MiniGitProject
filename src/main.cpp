#include <iostream>
#include <string>
#include <vector>


int initCommand();
int addCommand(const std::string& filename);
int commitCommand(const std::string& message);
int logCommits();
void handleBranchCommand(const std::string& repoPath, const std::vector<std::string>& args);
void handleCheckoutCommand(const std::string& repoPath, const std::vector<std::string>& args);

int main(int argc, char *argv[])
{
    
    if (argc < 2)
    {
        std::cout << "Usage: minigit <command>\n";
        return 1;
    }

    const std::vector<std::string> args(argv + 1, argv + argc);
    const std::string repoPath = ".minigit/";
    std::string command = argv[1];
    if (command == "init")
    {
        return initCommand();
    } else if (command == "add"){
        if(argc < 3) {
            std::cerr<< "Usage: ./minigit add <file>\n";
            return 1;
        }

        return addCommand(argv[2]);
    } else if(command == "commit"){
        std::string message;
        if(argc >= 4 && std::string(argv[2]) == "-m"){
            message = argv[3];
        } else {
            std:: cerr << "Usage: ./minigit commit -m \"message\"\n";
            return 1;
        }
        return commitCommand(message);
    } else if (command == "log") {
        return logCommits();
    } else if (command == "branch") {
        handleBranchCommand(repoPath, args);
    } else if (command == "checkout") {
        handleCheckoutCommand(repoPath, args);
    } else {
        std::cout << "Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}