cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_definitions(-DUNICODE -D_UNICODE)

set(SM_HEADER_FILES ShaderDataType.h ShaderConfig.h ShaderLib.h glsl2spv.h ShaderMaker.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp ShaderLib.cpp ShaderDataType.cpp ShaderConfigParse.cpp ShaderMaker.cpp ForwardShading.cpp)

IF(CM_BUILD_GUI_TOOLS)
    set(IDENTIFIER "com.hyzgame.shadermaker")

#    target_link_libraries(ShaderMaker PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
ENDIF(CM_BUILD_GUI_TOOLS)

set(VULKAN_SPIRV_LIBS   GenericCodeGen
                        glslang
                        HLSL
                        MachineIndependent
                        OGLCompiler
                        OSDependent
                        SPIRV
                        SPIRV-Tools
                        SPIRV-Tools-opt
                        spirv-cross-core)

add_executable(ShaderMaker ${SM_HEADER_FILES} ${SM_SOURCE_FILES})
target_link_libraries(ShaderMaker PRIVATE CMCore CMPlatform CMUtil ${VULKAN_SPIRV_LIBS})

add_executable(ShaderCompiler ShaderCompiler.cpp glsl2spv.h glsl2spv.cpp VKShaderParse.h)

target_link_libraries(ShaderCompiler PRIVATE    CMCore 
                                                CMPlatform
                                                CMUtil
                                                CMSceneGraph
                                                ${VULKAN_SPIRV_LIBS})

if(UNIX)
    target_link_libraries(ShaderMaker PRIVATE jsoncpp expat dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
    target_link_libraries(ShaderCompiler PRIVATE dl ${RENDER_LIBRARY} ${Vulkan_LIBRARY})
endif()

IF(WIN32)
    set_debugger_directory(ShaderMaker ${CMAKE_CURRENT_SOURCE_DIR})
    set_debugger_directory(ShaderCompiler ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()
