cmake_minimum_required(VERSION 2.8.4)
project(csci3150_asg3_release)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(SOURCE_FILES_PTHREAD
    asg3-pthread.c util.c)

set(SOURCE_FILES_OPENMP
    asg3-openmp.c util.c)

set(SOURCE_FILES_PTHREAD_V2
        v2/asg3-pthread.c
        v2/util.c)

set(SOURCE_FILES_OPENMP_V2
        v2/asg3-openmp.c
        v2/util.c)

add_executable(csci3150_asg3_release_pthread ${SOURCE_FILES_PTHREAD})
add_executable(csci3150_asg3_release_openmp ${SOURCE_FILES_OPENMP})
add_custom_target(csci3150_asg3_release_pthread_make ALL COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

add_executable(csci3150_asg3_release_pthread_v2 ${SOURCE_FILES_PTHREAD_V2})
add_executable(csci3150_asg3_release_openmp_v2 ${SOURCE_FILES_OPENMP_V2})
add_custom_target(csci3150_asg3_release_omp_make ALL COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})
