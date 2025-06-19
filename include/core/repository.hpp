#include "core/commit.hpp"
#include <string>
#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP
using namespace std;
namespace mgit
{
    class Repository
    {
    public:
        Repository();

        bool isInitialized();
        Commit getHeadCommit();
        Commit getCommitById(const string& hash);
        
    public:
        static void init();
    public:
        static void add(const string& file);
    public:
        static int commit(const string& message);
    public:
        static int logCommits();
    
    private:
        string mgitDir;
        string commitsDir;
        string headFile;

        string readFile(const string& path) const;
        Commit loadCommitFromFile(const string& path) const;
    
    };
}

#endif