cmake_minimum_required(VERSION 3.6)
project(csci3150_asg3_release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES_PTHREAD
    asg3-pthread.c)

set(SOURCE_FILES_OPENMP
    asg3-openmp.c)

add_executable(csci3150_asg3_release_pthread ${SOURCE_FILES_PTHREAD})
add_executable(csci3150_asg3_release_openmp ${SOURCE_FILES_OPENMP})