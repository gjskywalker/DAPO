add_library(HI_find_function STATIC HI_find_function.cpp)

target_include_directories(HI_find_function
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_find_function
)

target_link_libraries(HI_find_function
  PUBLIC
    HI_print
)
