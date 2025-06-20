#ifndef HASHING_HPP
#define HASHING_HPP

#include <string>
#include <openssl/sha.h>
#include <iomanip>
using namespace std;
namespace mgit {
    string generateHash(const string& content);
}

#endif
