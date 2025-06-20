#ifndef COMMIT_HPP
#define COMMIT_HPP

#include<string>
#include<map>
#include<vector>
using namespace std;
namespace mgit{
    struct Commit {
        string hash;
        string parent;
        string message;
        string timestamp;
        map<string,string> files;

        string serialize() const;// we make const at last because we are not changing the state of the object
        static Commit deserialize(const string& content);
    };
}


#endif