#include "core/commit.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace mgit
{
    std::string Commit::serialize() const
    {
        json j;
        j["hash"] = hash;
        j["parent"] = parent;
        j["message"] = message;
        j["timestamp"] = timestamp;
        j["files"] = files;
        return j.dump(4);
    }

    Commit Commit::deserialize(const std::string &content)
    {
        Commit c;
        json j = json::parse(content);
        c.hash = j.value("hash", "");
        c.parent = j["parent"];
        c.message = j["message"];
        c.timestamp = j["timestamp"];
        c.files = j["files"].get<std::map<std::string, std::string>>();
        return c;
    }
}