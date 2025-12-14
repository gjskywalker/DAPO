add_library(HI_duplicate_inst_rm STATIC HI_duplicateinstrm.cpp)

target_include_directories(HI_duplicate_inst_rm
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_duplicate_inst_rm
)

target_link_libraries(HI_duplicate_inst_rm
  PUBLIC
    HI_print
)
