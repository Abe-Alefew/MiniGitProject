#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include "utils/hashing.hpp"
using namespace std;


namespace mgit {

string generateHash(const string& content) {
    // In OpenSSL, SHA_DIGEST_LENGTH is the length of the SHA-1 hash in bytes (20 bytes). that is because SHA-1 produces a 160-bit hash, which is 20 bytes long.
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(content.c_str()), content.size(), hash);

    stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        ss << hex << setw(2) << setfill('0') << (int)hash[i]; // this converts each byte to a two-digit hexadecimal representation and appends to the stringstream
    return ss.str();
}

}
