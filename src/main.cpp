#include <argparse/argparse.hpp>

int main(int argc, char *argv[]) {
    argparse::ArgumentParser parser{"pupil_client"};

    try {
        parser.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}