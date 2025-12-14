add_library(HI_loop_information_collect STATIC HI_loop_information_collect.cpp)

target_include_directories(HI_loop_information_collect
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_loop_information_collect
)

target_link_libraries(HI_loop_information_collect
  PUBLIC
    HI_print
)
