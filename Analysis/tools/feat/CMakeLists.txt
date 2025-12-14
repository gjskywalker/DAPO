add_executable(feature_exctractor feature_exctractor.cpp)

target_link_libraries(feature_exctractor
	PRIVATE
		HI_feature_extractor
		HI_duplicate_inst_rm
		HI_find_function
		HI_loop_information_collect
		HI_separate_const_offset_from_gep
		HI_sys_exec
		HI_print
		HI_string_process
		${DAPO_COMMON_LINK_LIBS}
)

list(APPEND DAPO_TOOL_TARGETS feature_exctractor)
set(DAPO_TOOL_TARGETS "${DAPO_TOOL_TARGETS}" PARENT_SCOPE)
