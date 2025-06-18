#include <iostream>
#include <string>


int initCommand();
int addCommand(const std::string& filename);
int commitCommand(const std::string& message);
int logCommits();

int main(int argc, char *argv[])
{
    
    if (argc < 2)
    {
        std::cout << "Usage: minigit <command>\n";
        return 1;
    }
    
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
    }


    std::cout << "Unknown command: " << command << "\n";
    return 1;
}