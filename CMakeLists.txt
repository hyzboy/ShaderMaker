cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_subdirectory(SPIRV-Cross)
add_subdirectory(SPIRV-Cross/external/glslang)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(SPIRV-Cross/external/glslang)

set(SM_HEADER_FILES ShaderDataType.h ShaderConfig.h ShaderLib.h glsl2spv.h ShaderMaker.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp ShaderLib.cpp ShaderDataType.cpp ShaderConfigParse.cpp ShaderMaker.cpp ForwardShading.cpp)

add_executable(ShaderMaker ${SM_HEADER_FILES} ${SM_SOURCE_FILES})
add_executable(ShaderCompiler ShaderCompiler.cpp glsl2spv.h glsl2spv.cpp)

target_link_libraries(ShaderMaker PRIVATE CMCore CMPlatform CMUtil glslang SPIRV OGLCompiler OSDependent)
target_link_libraries(ShaderCompiler PRIVATE CMCore CMPlatform CMUtil glslang SPIRV OGLCompiler OSDependent)

if(UNIX)
    target_link_libraries(ShaderMaker PRIVATE jsoncpp expat dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
    target_link_libraries(ShaderCompiler PRIVATE dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
endif()

IF(WIN32)
    set_target_properties(ShaderMaker PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(ShaderCompiler PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()
