﻿cmake_minimum_required(VERSION 3.0)

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

SET(GLSL_COMPILER_SOURCE	GLSLCompiler/GLSLCompiler.h
                            GLSLCompiler/GLSLCompiler.cpp)

SET(VK_SHADER_COMMON_SOURCE vulkan/VKShaderCommon.h
                            vulkan/VKShaderCommon.cpp)

SOURCE_GROUP("Vulkan Shader Common" FILES ${VK_SHADER_COMMON_SOURCE})

SET(GLSLParse_SOURCE    GLSLParse/GLSLToken.h
                        GLSLParse/GLSLTokenizer.h
                        GLSLParse/GLSLTokenizer.cpp
                        GLSLParse/GLSLParse.cpp)

add_executable(ShaderParse GLSLParse/ShaderParseTest.cpp ${GLSLParse_SOURCE})
shader_maker_project(ShaderParse)

SET(SHADER_FILE_SOURCE  GLSLParse/ShaderSourceLocation.h
                        GLSLParse/ShaderStruct.h
                        GLSLParse/ShaderConstVarying.h
                        GLSLParse/ShaderFunction.h
                        GLSLParse/ShaderFile.h)

SOURCE_GROUP("Shader Lib\\Shader File" FILES ${SHADER_FILE_SOURCE})

SET(INFO_OUTPUT_SOURCE      InfoOutput/InfoOutput.h
                            InfoOutput/InfoOutput.cpp)

SOURCE_GROUP("Info Output" FILES ${INFO_OUTPUT_SOURCE})

SET(SHADER_LIBRARY_SOURCE   ShaderLibrary/ShaderLib.h
                            ShaderLibrary/ShaderLib.cpp
                            ShaderLibrary/ShaderLibParse.h
                            
                            ShaderLibrary/XMLShader.h
                            ShaderLibrary/XMLShader.cpp
                            ShaderLibrary/SLP_Folder.cpp
                            ShaderLibrary/SLP_Files.cpp
                            ShaderLibrary/SLP_Root.cpp
                            ShaderLibrary/SLPF_RawGLSL.cpp
                            ShaderLibrary/SLPF_XmlGLSL.cpp
                            ShaderLibrary/LoadVarying.cpp)

SOURCE_GROUP("GLSL Compiler" FILES ${GLSL_COMPILER_SOURCE})
SOURCE_GROUP("Shader Lib" FILES ${SHADER_LIBRARY_SOURCE})
SOURCE_GROUP("Shader Lib\\GLSL Parse" FILES ${GLSLParse_SOURCE})

SET(MATERIAL_COMPILER_SOURCE    MaterialCompiler/ShaderMaker.cpp
                                MaterialCompiler/ParseXMLShader.cpp
                                MaterialCompiler/ParseXMLMaterial.cpp
                                MaterialCompiler/ParseMaterial.cpp
                                MaterialCompiler/ParseMaterialShader.cpp
                                MaterialCompiler/SaveMaterial.cpp)

SOURCE_GROUP("Material Compiler" FILES ${MATERIAL_COMPILER_SOURCE})

add_executable(MaterialCompiler MaterialCompiler/MaterialCompiler.cpp
                                ${MATERIAL_COMPILER_SOURCE}
                                ${GLSL_COMPILER_SOURCE}
                                ${VK_SHADER_COMMON_SOURCE}
                                ${SHADER_LIBRARY_SOURCE}
                                ${INFO_OUTPUT_SOURCE})

add_executable(ShaderCompiler ShaderCompiler/ShaderCompiler.cpp ${GLSL_COMPILER_SOURCE} ${VK_SHADER_COMMON_SOURCE})

add_executable(PipelineCompiler PipelineCompiler/PipelineCompiler.cpp 
                                PipelineCompiler/LoadXMLPipeline.cpp)

shader_maker_project(ShaderCompiler)
shader_maker_project(PipelineCompiler)
shader_maker_project(MaterialCompiler)

IF(CM_BUILD_GUI_TOOLS)
    set(IDENTIFIER "com.hyzgame.materialwriter")

    SET(MW_BASIC_FILES  gui/TypeDefine.h
                        gui/ConfigData.cpp
                        gui/ConfigData.h
                        gui/QStringInfoOutput.cpp
                        gui/QStringInfoOutput.h)

    SET(MW_WIDGET_FILES gui/GLSLTextEdit.cpp
                        gui/GLSLTextEdit.h                        
                        gui/WI_EditorTreeWidgetItem.h
                        gui/W_EditorWidget.h
                        gui/W_ShaderEditor.h
                        gui/W_ShaderEditor.cpp
                        gui/W_MaterialEditor.cpp
                        gui/W_MaterialEditor.h
                        gui/W_XMLShaderEditor.cpp
                        gui/W_XMLShaderEditor.h
                        gui/XMLTextEdit.cpp
                        gui/XMLTextEdit.h
                        gui/W_XMLEditor.cpp
                        gui/W_XMLEditor.h
                        )

    SET(MW_DIALOG_FILES gui/DLG_About.cpp
                        gui/DLG_About.h
                        gui/DLG_Config.cpp
                        gui/DLG_Config.h
                        gui/DLG_GUIStyle.cpp
                        gui/DLG_GUIStyle.h
                        gui/DLG_CreateMaterial.cpp
                        gui/DLG_CreateMaterial.h)

    SET(MW_MAIN_FILES   gui/MaterialWriter.cpp
                        gui/MaterialWriter.qrc
                        gui/MainWindowMenu.cpp
                        gui/MainWindow.cpp
                        gui/MainWindow.h)

    SET(MATERIAL_WRITER_SOURCE  gui/TP_ShaderLibrary.cpp
                                gui/TP_ShaderLibrary.h
                                gui/TP_Varying.cpp
                                gui/TP_Varying.h
                                gui/TP_Struct.cpp
                                gui/TP_Struct.h
                                gui/TP_Raw.cpp
                                gui/TP_Raw.h
                                gui/TP_Module.cpp
                                gui/TP_Module.h
                                gui/TP_ShaderFileTree.cpp
                                gui/TP_ShaderFileMenu.cpp
                                gui/TP_ShaderFile.cpp
                                gui/TP_ShaderFile.h
                                gui/TP_MaterialLibraryFileTree.cpp
                                gui/TP_MaterialLibrary.cpp
                                gui/TP_MaterialLibrary.h
                                gui/ShaderLibTree.cpp
                                gui/ShaderLibTree.h)

    source_group("GUI" FILES ${MATERIAL_WRITER_SOURCE})
    source_group("GUI\\Basic" FILES ${MW_BASIC_FILES})
    source_group("GUI\\Widget" FILES ${MW_WIDGET_FILES})
    source_group("GUI\\Dialog" FILES ${MW_DIALOG_FILES})
    source_group("GUI\\MainWindow" FILES ${MW_MAIN_FILES})

    add_executable(MaterialWriter   ${MATERIAL_WRITER_SOURCE} ${MW_BASIC_FILES} ${MW_WIDGET_FILES} ${MW_DIALOG_FILES} ${MW_MAIN_FILES}

                                    ${MATERIAL_COMPILER_SOURCE}
                                    ${GLSLParse_SOURCE}
                                    ${SHADER_FILE_SOURCE}
                                    ${GLSL_COMPILER_SOURCE}
                                    ${VK_SHADER_COMMON_SOURCE}
                                    ${SHADER_LIBRARY_SOURCE}
                                    ${INFO_OUTPUT_SOURCE}
                                    ${CM_QT_EXTRA_STYLE_RC_FILES})

    target_link_libraries(MaterialWriter PRIVATE CMQT)
    set_target_properties(MaterialWriter PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

    shader_maker_project(MaterialWriter)
ENDIF()
