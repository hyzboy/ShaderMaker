cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_subdirectory(glslang)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(glslang)

add_executable(ShaderMaker main.cpp glsl2spv.cpp glsl2spv.h)

target_link_libraries(ShaderMaker PRIVATE CMCore CMPlatform CMUtil glslang SPIRV)

if(UNIX)
    target_link_libraries(ShaderMaker PRIVATE jsoncpp expat dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
endif()
