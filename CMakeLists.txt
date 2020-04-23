cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_subdirectory(glslang)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(glslang)

set(SM_HEADER_FILES ShaderDataType.h ShaderConfig.h ShaderLib.h glsl2spv.h ShaderMaker.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp ShaderLib.cpp ShaderDataType.cpp ShaderConfigParse.cpp ShaderMaker.cpp ForwardShading.cpp)

add_executable(ShaderMaker ${SM_HEADER_FILES} ${SM_SOURCE_FILES})

target_link_libraries(ShaderMaker PRIVATE CMCore CMPlatform CMUtil glslang SPIRV OGLCompiler OSDependent)

if(UNIX)
    target_link_libraries(ShaderMaker PRIVATE jsoncpp expat dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
endif()
