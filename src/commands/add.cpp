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
    string blobPath = ".minigit/blobs/" + hash;

    // Ensure blobs directory exists
    fs::create_directories(".minigit/blobs/");

    // Write blob if not already present
    if (!fs::exists(blobPath))
    {
        writeBlob(blobPath, content);
        cout << "Blob stored: " << blobPath << "\n";
    }
    else
    {
        cout << "Blob already exists. Skipping write.\n";
    }

    // Update .minigit/index without duplicates
    ifstream indexIn(".minigit/index");
    ofstream tempOut(".minigit/index.tmp");
    string line;
    bool updated = false;

    while (getline(indexIn, line))
    {
        string existingFile = line.substr(0, line.find(' '));
        if (existingFile == filename)
        {
            tempOut << filename << " " << hash << "\n";
            updated = true;
        }
        else
        {
            tempOut << line << "\n";
        }
    }

    if (!updated)
    {
        tempOut << filename << " " << hash << "\n";
    }

    indexIn.close();
    tempOut.close();
    fs::rename(".minigit/index.tmp", ".minigit/index");

    cout << "File added to staging area.\n";
}
