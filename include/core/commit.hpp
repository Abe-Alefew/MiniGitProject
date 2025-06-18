#ifndef COMMIT_HPP
#define COMMIT_HPP

#include<string>
#include<map>
#include<vector>

namespace mgit{
    struct Commit {
        std::string hash;
        std::string parent;
        std::string message;
        std::string timestamp;
        std::map<std::string, std::string> files;

        std::string serialize() const;
        static Commit deserialize(const std::string& content);
    };
}


#endif