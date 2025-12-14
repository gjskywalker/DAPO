set(DAPO_TOOL_TARGETS "")

add_subdirectory(feat)
add_subdirectory(qor)

set(DAPO_TOOL_TARGETS "${DAPO_TOOL_TARGETS}" PARENT_SCOPE)
