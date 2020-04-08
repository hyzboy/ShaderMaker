cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_subdirectory(glslang)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(glslang)

set(SM_HEADER_FILES ShaderDataType.h glsl2spv.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp)

add_executable(ShaderMaker ${SM_HEADER_FILES} ${SM_SOURCE_FILES})

target_link_libraries(ShaderMaker PRIVATE CMCore CMPlatform CMUtil glslang SPIRV OGLCompiler OSDependent)

if(UNIX)
    target_link_libraries(ShaderMaker PRIVATE jsoncpp expat dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
endif()
