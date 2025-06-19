#include "core/repository.hpp"
#include "core/commit.hpp"
#include "core/blob.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

namespace mgit
{
    string getCurrentTimeStamp();
    string generateHash(const string &content);

    Repository::Repository()
    {
        mgitDir = ".minigit";
        commitsDir = mgitDir + "/commits";
        headFile = mgitDir + "/HEAD";
    }

    bool Repository::isInitialized()
    {
        return fs::exists(mgitDir) &&
               fs::exists(commitsDir) &&
               fs::exists(headFile) &&
               fs::exists(mgitDir + "/branches/main");
    }

    Commit Repository::getHeadCommit()
    {
        if (!fs::exists(headFile)) return {};
        
        string headId = readFile(headFile);
        if (headId.empty()) return {};

        string path = commitsDir + "/" + headId + ".json";
        return fs::exists(path) ? loadCommitFromFile(path) : Commit{};
    }

    Commit Repository::getCommitById(const string &hash)
    {
        string path = commitsDir + "/" + hash + ".json";
        return fs::exists(path) ? loadCommitFromFile(path) : Commit{};
    }

    string Repository::readFile(const string &path) const
    {
        ifstream in(path);
        if (!in) return "";
        return string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    }

    Commit Repository::loadCommitFromFile(const string &path) const
    {
        ifstream in(path);
        if (!in) return Commit{};
        string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        return Commit::deserialize(content);
    }

    void Repository::init()
    {
        try
        {
            if (fs::exists(".minigit"))
            {
                cout << "Repository already initialized.\n";
                return;
            }

            fs::create_directories(".minigit/branches");
            fs::create_directories(".minigit/commits");
            fs::create_directories(".minigit/blobs");

            // HEAD points to the default branch
            ofstream(".minigit/HEAD") << "ref: branches/main";
            // Create an empty branch 'main' initially
            ofstream(".minigit/branches/main") << "";
            ofstream(".minigit/staging_area.json") << "{}";

            cout << "Initialized empty miniGit repository.\n";
        }
        catch (const std::exception &e)
        {
            cout << "Error initializing repository: " << e.what() << '\n';
        }
    }

    void Repository::add(const std::string &file)
    {
        if (!fs::exists(file))
        {
            cout << "File not found: " << file << "\n";
            return;
        }

        string blob_hash = Blob::create(file);

        json staging;
        ifstream in(".minigit/staging_area.json");
        if (in)
            in >> staging;

        staging[file] = blob_hash;

        ofstream out(".minigit/staging_area.json");
        out << staging.dump(4);

        cout << "Added " << file << " to staging area.\n";
    }

    int Repository::commit(const string &message)
    {
        mgit::Repository repo;

        Commit currHEAD = repo.getHeadCommit();
        string currHEADHash = currHEAD.hash;

        ifstream staging(".minigit/staging_area.json");
        json staged_files_json;
        if (!staging)
        {
            cout << "Staging area not found.\n";
            return 1;
        }
        staging >> staged_files_json;

        map<string, string> files;
        for (auto &[fname, blob] : staged_files_json.items())
        {
            files[fname] = blob;
        }

        Commit commit;
        commit.parent = currHEADHash;
        commit.message = message;
        commit.timestamp = getCurrentTimeStamp();
        commit.files = files;

        string content = commit.serialize();
        commit.hash = generateHash(content);
        content = commit.serialize(); // regenerate with hash
        // Save commit file
        ofstream(".minigit/commits/" + commit.hash + ".json") << content;

        // Read current branch from HEAD
        ifstream headIn(".minigit/HEAD");
        string headLine;
        getline(headIn, headLine);

        if (headLine.rfind("ref: ", 0) == 0){
            string branchPath = headLine.substr(5); // Remove "ref: "
            ofstream(".minigit/" + branchPath) << commit.hash;

            // Optionally, write HEAD again just to be safe
            ofstream(".minigit/HEAD") << "ref: " + branchPath;
        }
        else {
            cout << "Invalid HEAD. Cannot determine current branch.\n";
            return 1;
        }


        // Clear staging area
        ofstream(".minigit/staging_area.json") << "{}";

        cout << "Committed as " << commit.hash << "\n";
        return 0;
    }
    int Repository::logCommits()
{
    string current = Repository().readFile(".minigit/HEAD");

    while (!current.empty())
    {
        Commit commit = Repository().getCommitById(current);
        if (commit.hash.empty()) break;

        cout << "Commit: " << commit.hash << "\n"
             << "Message: " << commit.message << "\n"
             << "Timestamp: " << commit.timestamp << "\n"
             << "-----------------------------\n";

        current = commit.parent;
    }

    return 0;
}
}
