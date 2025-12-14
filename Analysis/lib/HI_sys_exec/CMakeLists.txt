add_library(HI_sys_exec STATIC HI_sys_exec.cpp)

target_include_directories(HI_sys_exec
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/../../include/HI_sys_exec
)

target_link_libraries(HI_sys_exec
	PUBLIC
		HI_print
)
