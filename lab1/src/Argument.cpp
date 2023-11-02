#include <iostream>
#include <cassert>
#include <sstream>
#include "Argument.h"


char const* Argument::fetch_argument(int argc, char const* const argv[], size_t argn) {
    if (argc < argn + 1) {
        std::cerr << argn << " argument is absent" << std::endl;
        assert(0);
    }

    return argv[argn];
}

std::size_t Argument::to_size_t(const char* arg) {
    std::stringstream stream(arg);
    std::size_t result;
    stream >> result;

    return result;
}
