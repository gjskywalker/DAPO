add_library(HI_feature_extractor STATIC HI_feature_extractor.cc)

target_include_directories(HI_feature_extractor
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_feature_extractor
)

target_link_libraries(HI_feature_extractor
  PUBLIC
    HI_print
)
