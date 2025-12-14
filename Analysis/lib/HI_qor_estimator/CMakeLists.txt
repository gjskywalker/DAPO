add_library(
	HI_qor_estimator
	STATIC
		HI_qor_estimator.cc
		HI_qor_estimator_blockevaluation.cc
		HI_qor_estimator_bramaccesstracer.cc
		HI_qor_estimator_configparse.cc
		HI_qor_estimator_functionevaluation.cc
		HI_qor_estimator_instructionevaluation.cc
		HI_qor_estimator_loopevaluation.cc
		HI_instruction_files.cc
		clock_info.cc
)

target_include_directories(HI_qor_estimator
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_qor_estimator
)

target_link_libraries(HI_qor_estimator
  PUBLIC
    HI_print
    HI_string_process
)
