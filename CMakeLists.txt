cmake_minimum_required(VERSION 3.0)

project(ShaderMaker)

add_definitions(-DUNICODE -D_UNICODE)

SET(SHADER_COMPILER_LIBRARY CMCore
                            CMPlatform
                            CMUtil
                            CMSceneGraph)

IF(WIN32)
    find_package(expat CONFIG REQUIRED)
    find_package(jsoncpp CONFIG REQUIRED)

    SET(SHADER_COMPILER_LIBRARY ${SHADER_COMPILER_LIBRARY} expat::expat jsoncpp_lib)
ENDIF()

macro(shader_maker_project project_name)
    target_link_libraries(${project_name} PRIVATE ${SHADER_COMPILER_LIBRARY})

    if(UNIX)
        target_link_libraries(${project_name} PRIVATE dl)
    endif()

    IF(WIN32)
        set_debugger_directory(${project_name} ${CMAKE_CURRENT_SOURCE_DIR})
    ENDIF()

    set_property(TARGET ${project_name} PROPERTY FOLDER "CM/Tools/ShaderMaker")
endmacro()

set(SM_HEADER_FILES ShaderDataType.h ShaderConfig.h ShaderLib.h glsl2spv.h ShaderMaker.h)
set(SM_SOURCE_FILES main.cpp glsl2spv.cpp ShaderLib.cpp ShaderDataType.cpp ShaderConfigParse.cpp ShaderMaker.cpp ForwardShading.cpp)

SET(GLSL_COMPILER_SOURCE	GLSLCompiler.h
                            GLSLCompiler.cpp)

SET(VK_SHADER_COMMON_SOURCE VKShaderCommon.h
                            VKShaderCommon.cpp)

SOURCE_GROUP("Vulkan Shader Common" FILES ${VK_SHADER_COMMON_SOURCE})

SET(GLSLParse_SOURCE    GLSLToken.h
                        GLSLTokenizer.h
                        GLSLTokenizer.cpp
                        GLSLParse.cpp)

add_executable(ShaderParse ShaderParseTest.cpp ${GLSLParse_SOURCE})
shader_maker_project(ShaderParse)

SET(SHADER_MODULE_SOURCE    ShaderSourceLocation.h
                            ShaderStruct.h
                            ShaderConstVarying.h
                            ShaderFunction.h
                            ShaderModule.h)

SOURCE_GROUP("Shader Lib\\Shader Module" FILES ${SHADER_MODULE_SOURCE})

SET(SHADER_LIBRARY_SOURCE   ShaderLib.h
                            ShaderLib.cpp
                            ShaderLibParse.h
                            InfoOutput.h
                            InfoOutput.cpp
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
SOURCE_GROUP("Shader Lib\\GLSL Parse" FILES ${GLSLParse_SOURCE})

SET(MATERIAL_COMPILER_SOURCE    ShaderMaker.cpp
                                ParseXMLShader.cpp
                                ParseXMLMaterial.cpp
                                SaveMaterial.cpp)

add_executable(MaterialCompiler MaterialCompiler.cpp
                                ${MATERIAL_COMPILER_SOURCE}
                                ${GLSL_COMPILER_SOURCE}
                                ${VK_SHADER_COMMON_SOURCE}
                                ${SHADER_LIBRARY_SOURCE})

add_executable(ShaderCompiler ShaderCompiler.cpp ${GLSL_COMPILER_SOURCE} ${VK_SHADER_COMMON_SOURCE})

add_executable(PipelineCompiler PipelineCompiler.cpp LoadXMLPipeline.cpp)

shader_maker_project(ShaderCompiler)
shader_maker_project(PipelineCompiler)
shader_maker_project(MaterialCompiler)

IF(CM_BUILD_GUI_TOOLS)
    set(IDENTIFIER "com.hyzgame.materialwriter")
    SET(MATERIAL_WRITER_SOURCE  gui/MaterialWriter.cpp
                                gui/MaterialWriter.qrc
                                gui/TypeDefine.h
                                gui/GLSLTextEdit.cpp
                                gui/GLSLTextEdit.h
                                gui/ConfigData.cpp
                                gui/ConfigData.h
                                gui/QStringInfoOutput.cpp
                                gui/QStringInfoOutput.h
                                gui/TP_ShaderLibrary.cpp
                                gui/TP_ShaderLibrary.h
                                gui/TP_Varying.cpp
                                gui/TP_Varying.h
                                gui/TP_Struct.cpp
                                gui/TP_Struct.h
                                gui/TP_Raw.cpp
                                gui/TP_Raw.h
                                gui/TP_Module.cpp
                                gui/TP_Module.h
                                gui/TP_MaterialLibraryFileTree.cpp
                                gui/TP_MaterialLibrary.cpp
                                gui/TP_MaterialLibrary.h
                                gui/W_EditorWidget.h
                                gui/W_MaterialEditor.cpp
                                gui/W_MaterialEditor.h
                                gui/W_XMLShaderEditor.cpp
                                gui/W_XMLShaderEditor.h
                                gui/MainWindowMenu.cpp
                                gui/MainWindow.cpp
                                gui/MainWindow.h
                                gui/DLG_PathConfig.cpp
                                gui/DLG_PathConfig.h
                                gui/DLG_About.cpp
                                gui/DLG_About.h
                                gui/ShaderLibTree.cpp
                                gui/ShaderLibTree.h)

    source_group("GUI" FILES ${MATERIAL_WRITER_SOURCE})

    add_executable(MaterialWriter   ${MATERIAL_WRITER_SOURCE}
                                    ${MATERIAL_COMPILER_SOURCE}
                                    ${GLSLParse_SOURCE}
                                    ${SHADER_MODULE_SOURCE}
                                    ${GLSL_COMPILER_SOURCE}
                                    ${VK_SHADER_COMMON_SOURCE}
                                    ${SHADER_LIBRARY_SOURCE})

    target_link_libraries(MaterialWriter PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
    set_target_properties(MaterialWriter PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

    shader_maker_project(MaterialWriter)
ENDIF()
