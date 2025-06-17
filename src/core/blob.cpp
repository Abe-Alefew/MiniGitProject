#include <fstream>
#include <string>
#include <iostream>

using namespace std;

void writeBlob(const string &path, const string &content)
{
    ofstream outFile(path);
    if (!outFile)
    {
        cerr << "Error: Unable to write blob to " << path << "\n";
        return;
    }
    outFile << content;
    outFile.close();
}

string readFileContents(const string &filename)
{
    ifstream inFile(filename);
    string content((istreambuf_iterator<char>(inFile)),
                   istreambuf_iterator<char>());
    return content;
}
