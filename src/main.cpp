#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <mpack/mpack.h>
#include <zmqpp/zmqpp.hpp>

#include "pupil_data_formats.h"

int main(int argc, char *argv[]) {
    std::cout << "pupil_client\n"
                 "zmqpp version: "
              << zmqpp::version() << "\n\n";

    const std::string endpoint_addr = "tcp://localhost";
    const int remote_port = 50020;

    zmqpp::context context;
    zmqpp::socket pupil_remote(context, zmqpp::socket_type::request);
    pupil_remote.connect(endpoint_addr + ":" + std::to_string(remote_port));

    std::cout << "Request 'SUB_PORT' for reading data\n";
    zmqpp::message message;
    message << "SUB_PORT";
    pupil_remote.send(message);

    std::string sub_port;
    pupil_remote.receive(sub_port);
    std::cout << "Reply " << sub_port << "\n\n";

    zmqpp::socket subscriber(context, zmqpp::socket_type::subscribe);
    subscriber.connect(endpoint_addr + ":" + sub_port);
    subscriber.subscribe("custom.");
    subscriber.subscribe("gaze.");

    while (true) {
        zmqpp::message sub_msg;
        if (!subscriber.receive(sub_msg, true)) {
            std::cout << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        } else {
            std::cout << std::endl;
        }

        assert((sub_msg.parts() == 2) && "expect topic and payload");
        std::string topic;
        sub_msg.get(topic, 0);

        mpack_tree_t tree;
        mpack_tree_init_data(&tree,
                             static_cast<const char *>(sub_msg.raw_data(1)),
                             sub_msg.size(1));
        mpack_tree_parse(&tree);
        mpack_node_t payload = mpack_tree_root(&tree);
        if (topic.find("custom.") == 0) { // like .starts_with()
            mpack_node_t hello = mpack_node_map_cstr(payload, "hello");
            std::string hello_val(mpack_node_str(hello),
                                  mpack_node_strlen(hello));

            if (mpack_tree_error(&tree) != mpack_ok) {
                std::cerr << "Error " << mpack_tree_error(&tree)
                          << " when parsing custom payload\n";
                continue;
            }

            std::cout << "Custom topic '" << topic
                      << "' with payload { 'hello': '" << hello_val << "' }\n";
        } else if (topic == std::string{"gaze.3d.01."}) {
            GazeBinocular gaze(payload);

            if (mpack_tree_error(&tree) != mpack_ok) {
                std::cerr << "Error " << mpack_tree_error(&tree)
                          << " when parsing binocular gaze datum\n";
                continue;
            }

            std::cout << gaze << "\n";
        }
    }

    return EXIT_SUCCESS;
}