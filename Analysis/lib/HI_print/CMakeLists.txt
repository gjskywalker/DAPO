add_library(HI_print STATIC HI_print.cc)

target_include_directories(HI_print
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_print
)
