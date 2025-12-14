add_library(HI_string_process STATIC HI_string_process.cc)

target_include_directories(HI_string_process
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_string_process
)
