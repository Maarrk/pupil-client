cmake_minimum_required(VERSION 3.20)
project(PupilClient)

option(BUILD_TESTS "Build tests for libzmq" OFF)

# install all dependencies locally, instead of system-wide
# this means that the project will need to be built twice the first time,
# but makes it easier to use the project without admin/root privileges
get_filename_component(LOCAL_INSTALL_PATH "${CMAKE_CURRENT_LIST_DIR}/build-install" ABSOLUTE)
set(CMAKE_INSTALL_PREFIX "${LOCAL_INSTALL_PATH}")
include_directories("${LOCAL_INSTALL_PATH}/include")

# ZeroMQ libraries for communication
add_subdirectory(libzmq)

set(ZEROMQ_INCLUDE_DIR "../libzmq/include")
set(ZMQPP_BUILD_STATIC 1)
set(ZMQPP_LIBZMQ_CMAKE 1)
add_subdirectory(zmqpp)

# Serialisation library for MessagePack
include_directories("mpack/src")
file(GLOB MPACK_SOURCE_FILES "mpack/src/mpack/*.c")

if(EXISTS "${LOCAL_INSTALL_PATH}")
    include_directories("src")
    add_executable(pupil_client
        src/main.cpp
        src/pupil_data_formats.cpp
        "${MPACK_SOURCE_FILES}")
    target_link_libraries(pupil_client zmqpp-static)
else()
    message(STATUS "Build and install project at least once to get dependencies in ${LOCAL_INSTALL_PATH}")
endif()