#ifndef BLOB_HPP
#define BLOB_HPP

#include <string>
using namespace std;

namespace mgit {

class Blob {
public:
    static string create(const string& filepath);
};

}

#endif