#include <iostream>

#include "zmqpp/zmqpp.hpp"

int main(int argc, char *argv[]) {
  std::cout << "pupil_client\n"
            << "zmqpp version: " << zmqpp::version() << std::endl;
  return EXIT_SUCCESS;
}