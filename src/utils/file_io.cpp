#include <fstream>
#include <sstream>
#include "utils/file_io.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
using namespace std;
using namespace std::chrono;
namespace mgit {

string getCurrentTimeStamp() {
    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

string readFile(const string& filepath) {
    ifstream in(filepath);
    stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}
}



