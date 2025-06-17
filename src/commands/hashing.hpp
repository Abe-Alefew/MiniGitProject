#ifndef HASHING_HPP
#define HASHING_HPP

#include <openssl/sha.h>
#include <iomanip>
#include <string>

std::string computeHash(const std::string& data);

#endif
