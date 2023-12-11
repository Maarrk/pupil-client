#include <iostream>

#include "zmqpp/zmqpp.hpp"

int main(int argc, char *argv[]) {
    std::cout << "zmqpp version: " << zmqpp::version() << std::endl;
    return EXIT_SUCCESS;
}