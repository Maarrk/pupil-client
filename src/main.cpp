#include <iostream>

#include "msgpack.hpp"
#include "zmqpp/zmqpp.hpp"

int main(int argc, char *argv[]) {
    std::cout << "msgpack-cxx version: " << msgpack_version() << std::endl;
    std::cout << "zmqpp version: " << zmqpp::version() << std::endl;
    return EXIT_SUCCESS;
}