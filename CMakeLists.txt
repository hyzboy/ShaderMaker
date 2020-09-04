cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_definitions(-DUNICODE -D_UNICODE)

set(SM_HEADER_FILES ShaderDataType.h ShaderConfig.h ShaderLib.h glsl2spv.h ShaderMaker.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp ShaderLib.cpp ShaderDataType.cpp ShaderConfigParse.cpp ShaderMaker.cpp ForwardShading.cpp)

IF(CM_BUILD_GUI_TOOLS)
    set(IDENTIFIER "com.hyzgame.shadermaker")

#    target_link_libraries(ShaderMaker PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
ENDIF(CM_BUILD_GUI_TOOLS)

SET(GLSL_COMPILER_SOURCE	GLSLCompiler.h
							GLSLCompiler.cpp)

SET(SHADER_LIBRARY_SOURCE   ShaderModule.h
                            ShaderLib.h
                            ShaderLib.cpp
                            ShaderLibParse.h
                            SLP_Folder.cpp
                            SLP_Files.cpp
                            SLP_Root.cpp
                            SLPF_RawGLSL.cpp
                            SLPF_XmlGLSL.cpp
                            SLPF_Varying.cpp)

SOURCE_GROUP("GLSL Compiler" FILES ${GLSL_COMPILER_SOURCE})
SOURCE_GROUP("Shader Lib" FILES ${SHADER_LIBRARY_SOURCE})

add_executable(ShaderMaker  ShaderMaker.cpp
                            ${GLSL_COMPILER_SOURCE}
                            ${SHADER_MODULE_SOURCE}
                            ${SHADER_LIBRARY_SOURCE})

add_executable(ShaderCompiler ShaderCompiler.cpp ${GLSL_COMPILER_SOURCE})

SET(SHADER_COMPILER_LIBRARY CMCore
                            CMPlatform
                            CMUtil
                            CMSceneGraph
                            libexpat)

target_link_libraries(ShaderMaker PRIVATE ${SHADER_COMPILER_LIBRARY})
target_link_libraries(ShaderCompiler PRIVATE ${SHADER_COMPILER_LIBRARY})

if(UNIX)
   target_link_libraries(ShaderMaker PRIVATE dl)
   target_link_libraries(ShaderCompiler PRIVATE dl)
endif()

IF(WIN32)
    set_debugger_directory(ShaderMaker ${CMAKE_CURRENT_SOURCE_DIR})
    set_debugger_directory(ShaderCompiler ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()
