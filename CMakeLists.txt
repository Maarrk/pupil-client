cmake_minimum_required(VERSION 3.20)
project(pupil_client)

# argparse requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(zmqpp REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} zmqpp::zmqpp)