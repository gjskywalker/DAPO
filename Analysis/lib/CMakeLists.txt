set(DAPO_ANALYSIS_LIBS "")

add_subdirectory(HI_print)
add_subdirectory(HI_string_process)
add_subdirectory(HI_sys_exec)
add_subdirectory(HI_duplicate_inst_rm)
add_subdirectory(HI_feature_extractor)
add_subdirectory(HI_find_function)
add_subdirectory(HI_loop_information_collect)
add_subdirectory(HI_separate_const_offset_from_gep)
add_subdirectory(HI_qor_estimator)

list(APPEND DAPO_ANALYSIS_LIBS
  HI_print
  HI_string_process
  HI_sys_exec
  HI_duplicate_inst_rm
  HI_feature_extractor
  HI_find_function
  HI_loop_information_collect
  HI_separate_const_offset_from_gep
  HI_qor_estimator
)

set(DAPO_ANALYSIS_LIBS "${DAPO_ANALYSIS_LIBS}" PARENT_SCOPE)
