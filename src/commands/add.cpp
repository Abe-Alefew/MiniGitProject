#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "../../include/core/blob.hpp"
#include "../../src/commands/hashing.hpp"

using namespace std;
namespace fs = std::filesystem;

void addFile(const string &filename)
{
    if (!fs::exists(filename))
    {
        cerr << "Error: File does not exist.\n";
        return;
    }

    string content = readFileContents(filename);
    string hash = computeHash(content);
    string blobPath = ".minigit/objects/" + hash;

    if (!fs::exists(blobPath))
    {
        writeBlob(blobPath, content);
        cout << "Blob stored: " << blobPath << "\n";
    }
    else
    {
        cout << "Blob already exists. Skipping write.\n";
    }

    ofstream indexFile(".minigit/index", ios::app);
    indexFile << filename << " " << hash << "\n";
    indexFile.close();

    cout << "File added to staging area.\n";
}
