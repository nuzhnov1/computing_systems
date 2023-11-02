#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <cstdlib>


namespace Argument {
    char const* fetch_argument(int argc, char const* const argv[], size_t argn);
    std::size_t to_size_t(const char* arg);
}


#endif //ARGUMENT_H
