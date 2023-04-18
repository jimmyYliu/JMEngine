
set(TARGET_NAME JMEngineRuntime)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

add_library (${TARGET_NAME}  ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${ENGINE_ROOT_DIR})

target_link_libraries(${TARGET_NAME} 
PUBLIC glfw
PUBLIC imgui
PUBLIC glm
PUBLIC spdlog
PUBLIC ${vulkan_lib})

