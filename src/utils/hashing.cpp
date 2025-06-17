#include <string>
#include <sstream>
#include <functional>

using namespace std;

string computeHash(const string &data)
{
    hash<string> hasher;
    size_t hashValue = hasher(data);

    stringstream ss;
    ss << hex << hashValue;
    return ss.str();
}
