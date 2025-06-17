#include <iostream>
#include "../src/commands/add.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: minigit add <filename>\n";
        return 1;
    }

    string command = argv[1];
    string filename = argv[2];

    if (command == "add")
    {
        addFile(filename);
    }
    else
    {
        cerr << "Unknown command.\n";
    }

    return 0;
}
