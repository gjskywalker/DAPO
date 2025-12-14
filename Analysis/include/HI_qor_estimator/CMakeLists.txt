set(_hi_qor_headers
	${CMAKE_CURRENT_SOURCE_DIR}/HI_qor_estimator.h
	${CMAKE_CURRENT_SOURCE_DIR}/HI_instruction_files.h
	${CMAKE_CURRENT_SOURCE_DIR}/clock_info.h
)
list(APPEND ANALYSIS_HEADER_FILES ${_hi_qor_headers})
set(ANALYSIS_HEADER_FILES "${ANALYSIS_HEADER_FILES}" PARENT_SCOPE)
