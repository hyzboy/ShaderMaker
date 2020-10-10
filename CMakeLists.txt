cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_definitions(-DUNICODE -D_UNICODE)

SET(SHADER_COMPILER_LIBRARY CMCore
                            CMPlatform
                            CMUtil
                            CMSceneGraph
                            libexpat)

macro(shader_maker_project project_name)
    target_link_libraries(${project_name} PRIVATE ${SHADER_COMPILER_LIBRARY})

    if(UNIX)
        target_link_libraries(${project_name} PRIVATE dl)
    endif()

    IF(WIN32)
        set_debugger_directory(${project_name}  ${CMAKE_CURRENT_SOURCE_DIR})
    ENDIF()

    set_property(TARGET ${project_name} PROPERTY FOLDER "ShaderMaker")
endmacro()

set(SM_HEADER_FILES ShaderDataType.h ShaderConfig.h ShaderLib.h glsl2spv.h ShaderMaker.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp ShaderLib.cpp ShaderDataType.cpp ShaderConfigParse.cpp ShaderMaker.cpp ForwardShading.cpp)

SET(GLSL_COMPILER_SOURCE	GLSLCompiler.h
                            GLSLCompiler.cpp)

SET(SHADER_LIBRARY_SOURCE   ShaderLib.h
                            ShaderLib.cpp
                            ShaderLibParse.h
                            XMLShader.h
                            XMLShader.cpp
                            SLP_Folder.cpp
                            SLP_Files.cpp
                            SLP_Root.cpp
                            SLPF_RawGLSL.cpp
                            SLPF_XmlGLSL.cpp
                            SLPF_Varying.cpp)

SOURCE_GROUP("GLSL Compiler" FILES ${GLSL_COMPILER_SOURCE})
SOURCE_GROUP("Shader Lib" FILES ${SHADER_LIBRARY_SOURCE})

SET(MATERIAL_COMPILER_SOURCE    ShaderMaker.cpp
                                ParseXMLShader.cpp
                                ParseXMLMaterial.cpp)

add_executable(MaterialCompiler MaterialCompiler.cpp
                                ${MATERIAL_COMPILER_SOURCE}
                                ${GLSL_COMPILER_SOURCE}
                                ${SHADER_LIBRARY_SOURCE})

add_executable(ShaderCompiler ShaderCompiler.cpp ${GLSL_COMPILER_SOURCE})

add_executable(PipelineCompiler PipelineCompiler.cpp LoadXMLPipeline.cpp)

shader_maker_project(ShaderCompiler)
shader_maker_project(PipelineCompiler)
shader_maker_project(MaterialCompiler)

IF(CM_BUILD_GUI_TOOLS)
    set(IDENTIFIER "com.hyzgame.materialbuilder")
    SET(MATERIAL_BUILDER_SOURCE gui/MaterialBuilder.cpp
                                gui/MaterialBuilder.qrc
                                gui/GLSLSyntaxHighlighter.cpp
                                gui/MainWindowMenu.cpp
                                gui/MainWindow.cpp
                                gui/MainWindowShaderLib.cpp
                                gui/MainWindow.h
                                gui/DLG_About.cpp
                                gui/DLG_About.h
                                gui/ShaderLibTree.cpp
                                gui/ShaderLibTree.h)

    source_group("GUI" FILES ${MATERIAL_BUILDER_SOURCE})

    add_executable(MaterialBuilder  ${MATERIAL_BUILDER_SOURCE}
                                    ${MATERIAL_COMPILER_SOURCE}
                                    ${GLSL_COMPILER_SOURCE}
                                    ${SHADER_LIBRARY_SOURCE})

    target_link_libraries(MaterialBuilder PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
    set_target_properties(MaterialBuilder PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

    shader_maker_project(MaterialBuilder)
ENDIF()
