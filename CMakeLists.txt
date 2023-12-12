cmake_minimum_required(VERSION 3.20)
project(PupilClient)

# install all dependencies locally, instead of system-wide
# this means that the project will need to be built twice the first time,
# but makes it easier to use the project without admin/root privileges
get_filename_component(LOCAL_INSTALL_PATH "${CMAKE_CURRENT_LIST_DIR}/build-install" ABSOLUTE)
set(CMAKE_INSTALL_PREFIX "${LOCAL_INSTALL_PATH}")
include_directories("${LOCAL_INSTALL_PATH}/include")

# ZeroMQ libraries for communication
add_subdirectory(libzmq)
include_directories("libzmq/include")

set(ZEROMQ_INCLUDE_DIR "../libzmq/include")
set(ZMQPP_BUILD_STATIC 1)
set(ZMQPP_LIBZMQ_CMAKE 1)
add_subdirectory(zmqpp)

if(EXISTS "${LOCAL_INSTALL_PATH}")
    add_executable(pupil_client src/main.cpp)
    target_link_libraries(pupil_client zmqpp-static)
else()
    message(STATUS "Build and install project at least once to get dependencies in ${LOCAL_INSTALL_PATH}")
endif()