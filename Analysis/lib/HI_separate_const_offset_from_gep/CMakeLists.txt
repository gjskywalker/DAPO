add_library(HI_separate_const_offset_from_gep STATIC HI_separate_const_offset_from_gep.cpp)

target_include_directories(HI_separate_const_offset_from_gep
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_separate_const_offset_from_gep
)

target_link_libraries(HI_separate_const_offset_from_gep
  PUBLIC
    HI_print
)
