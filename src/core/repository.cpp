#include "core/repository.hpp"
#include "core/commit.hpp"
#include "core/blob.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace mgit
{
    std::string getCurrentTimeStamp();
    std::string generateHash(const std::string &content);

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
        if (!fs::exists(headFile))
        {
            return {};
        }

        std::string headId = readFile(headFile);
        if (headId.empty()) return {};

        std::string path = commitsDir + "/" + headId + ".json";
        return fs::exists(path) ? loadCommitFromFile(path) : Commit{};
    }

    Commit Repository::getCommitById(const std::string &hash)
    {
        std::string path = commitsDir + "/" + hash + ".json";
        return fs::exists(path) ? loadCommitFromFile(path) : Commit{};
    }

    std::string Repository::readFile(const std::string &path) const
    {
        std::ifstream in(path);
        if (!in) return "";
        return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }

    Commit Repository::loadCommitFromFile(const std::string &path) const
    {
        std::ifstream in(path);
        if (!in) return Commit{};
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        return Commit::deserialize(content);
    }

    void Repository::init()
    {
        try
        {
            if (fs::exists(".minigit"))
            {
                std::cout << "Repository already initialized.\n";
                return;
            }

            fs::create_directories(".minigit/branches");
            fs::create_directories(".minigit/commits");
            fs::create_directories(".minigit/blobs");

            // HEAD points to the default branch
            std::ofstream(".minigit/HEAD") << "main";
            // Create an empty branch 'main' initially
            std::ofstream(".minigit/branches/main") << "";
            std::ofstream(".minigit/staging_area.json") << "{}";

            std::cout << "Initialized empty miniGit repository.\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error initializing repository: " << e.what() << '\n';
        }
    }

    void Repository::add(const std::string &file)
    {
        if (!fs::exists(file))
        {
            std::cerr << "File not found: " << file << "\n";
            return;
        }

        std::string blob_hash = Blob::create(file);

        json staging;
        std::ifstream in(".minigit/staging_area.json");
        if (in)
            in >> staging;

        staging[file] = blob_hash;

        std::ofstream out(".minigit/staging_area.json");
        out << staging.dump(4);

        std::cout << "Added " << file << " to staging area.\n";
    }

    int Repository::commit(const std::string &message)
    {
        mgit::Repository repo;

        Commit currHEAD = repo.getHeadCommit();
        std::string currHEADHash = currHEAD.hash;

        std::ifstream staging(".minigit/staging_area.json");
        json staged_files_json;
        if (!staging)
        {
            std::cerr << "Staging area not found.\n";
            return 1;
        }
        staging >> staged_files_json;

        std::map<std::string, std::string> files;
        for (auto &[fname, blob] : staged_files_json.items())
        {
            files[fname] = blob;
        }

        Commit commit;
        commit.parent = currHEADHash;
        commit.message = message;
        commit.timestamp = getCurrentTimeStamp();
        commit.files = files;

        std::string content = commit.serialize();
        commit.hash = generateHash(content);
        content = commit.serialize(); // regenerate with hash

        // Save new HEAD (hash) and commit file
        std::ofstream(".minigit/HEAD") << commit.hash;
        std::ofstream(".minigit/commits/" + commit.hash + ".json") << content;

        // Update branch reference (assume HEAD = "main")
        std::ofstream(".minigit/branches/main") << commit.hash;

        // Clear staging area
        std::ofstream(".minigit/staging_area.json") << "{}";

        std::cout << "Committed as " << commit.hash << "\n";
        return 0;
    }
    int Repository::logCommits()
{
    std::string current = Repository().readFile(".minigit/HEAD");

    while (!current.empty())
    {
        Commit commit = Repository().getCommitById(current);
        if (commit.hash.empty()) break;

        std::cout << "Commit: " << commit.hash << "\n"
                  << "Message: " << commit.message << "\n"
                  << "Timestamp: " << commit.timestamp << "\n"
                  << "-----------------------------\n";

        current = commit.parent;
    }

    return 0;
}
}
