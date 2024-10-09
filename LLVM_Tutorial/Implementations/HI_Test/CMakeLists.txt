cmake_minimum_required(VERSION 3.10)

find_package(LLVM REQUIRED CONFIG PATHS NO_DEFAULT_PATH)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

aux_source_directory(. DIR_HI_Test_SRCS)
add_library(HI_Test ${DIR_HI_Test_SRCS})

# project(Demo)
# add_executable(Demo A.cc B.cc C.cc)
# When there are many .cc files, the add_executable will become very lenthy, 
# then you can use aux_source_directory to do it easily.
# aux_source_directory(. DIR_FILE_DIRS)
# add_executable(Demo ${DIR_FILE_DIRS})