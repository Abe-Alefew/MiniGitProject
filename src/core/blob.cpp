#include "core/blob.hpp"
#include "utils/hashing.hpp"
#include "utils/file_io.hpp"

#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = filesystem;

namespace mgit {

string Blob::create(const string& filepath) {
    string content = readFile(filepath);  // read content
    string hash = generateHash(content);     // generate hash

    fs::path blob_path = ".minigit/blobs/" + hash;
    if (!fs::exists(blob_path)) {
        ofstream(blob_path) << content;
    }

    return hash;
}


}
