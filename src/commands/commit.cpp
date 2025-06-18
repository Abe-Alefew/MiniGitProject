#include "core/repository.hpp"

int commitCommand(const std::string& message){
    return mgit::Repository::commit(message);
}