
cmake_minimum_required(VERSION 3.10) 

project(EXTRACDFG)
find_package(LLVM REQUIRED CONFIG) 
# Needed to use support library 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17") 

include_directories(${LLVM_INCLUDE_DIRS}) 
add_definitions(${LLVM_DEFINITIONS})

include_directories(../HI_print)

aux_source_directory(. DIR_HI_EXTRACDFG_SRCS)
add_library(HI_ExtraCDFG SHARED ${DIR_HI_EXTRACDFG_SRCS})


# set(llvm_libs "")

