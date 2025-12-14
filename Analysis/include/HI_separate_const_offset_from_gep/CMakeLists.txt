set(_hi_separate_headers ${CMAKE_CURRENT_SOURCE_DIR}/HI_separate_const_offset_from_gep.h)
list(APPEND ANALYSIS_HEADER_FILES ${_hi_separate_headers})
set(ANALYSIS_HEADER_FILES "${ANALYSIS_HEADER_FILES}" PARENT_SCOPE)
