#include "core/repository.hpp" // using the header file for the Repository class
using namespace mgit;
int initCommand()
{
    Repository::init();
    return 0;
}