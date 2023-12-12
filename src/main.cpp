#include <iostream>

#include <zmqpp/zmqpp.hpp>

int main(int argc, char *argv[]) {
    std::cout << "pupil_client\n"
                 "zmqpp version: "
              << zmqpp::version() << "\n\n";

    const std::string endpoint = "tcp://localhost:50020";

    zmqpp::context context;
    zmqpp::socket pupil_remote(context, zmqpp::socket_type::req);
    pupil_remote.connect(endpoint);

    std::cout << "Request 'SUB_PORT' for reading data\n";
    zmqpp::message message;
    message << "SUB_PORT";
    pupil_remote.send(message);

    std::string sub_port;
    pupil_remote.receive(sub_port);

    std::cout << "Got reply " << sub_port << std::endl;

    return EXIT_SUCCESS;
}