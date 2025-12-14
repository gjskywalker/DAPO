set(ANALYSIS_HEADER_FILES "")

add_library(HIAnalysis_headers INTERFACE)
add_library(HIAnalysis::headers ALIAS HIAnalysis_headers)

target_link_libraries(HIAnalysis_headers INTERFACE HIAnalysis::framework)
target_include_directories(
	HIAnalysis_headers
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
)

add_subdirectory(HI_print)
add_subdirectory(HI_duplicate_inst_rm)
add_subdirectory(HI_feature_extractor)
add_subdirectory(HI_find_function)
add_subdirectory(HI_loop_information_collect)
add_subdirectory(HI_qor_estimator)
add_subdirectory(HI_separate_const_offset_from_gep)
add_subdirectory(HI_string_process)
add_subdirectory(HI_sys_exec)

if(ANALYSIS_HEADER_FILES)
	target_sources(HIAnalysis_headers INTERFACE ${ANALYSIS_HEADER_FILES})
  	set(_analysis_header_dirs)
  	foreach(_hdr IN LISTS ANALYSIS_HEADER_FILES)
  		get_filename_component(_hdr_dir "${_hdr}" DIRECTORY)
  		list(APPEND _analysis_header_dirs "${_hdr_dir}")
  	endforeach()
  	list(REMOVE_DUPLICATES _analysis_header_dirs)
  	target_include_directories(HIAnalysis_headers INTERFACE ${_analysis_header_dirs})
endif()

set(ANALYSIS_HEADER_FILES "${ANALYSIS_HEADER_FILES}" PARENT_SCOPE)
