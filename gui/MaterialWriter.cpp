#include<QApplication>
#include<QFont>
#include<QStyle>
#include<QDesktopWidget>
#include<QMessageBox>
#include"MainWindow.h"
#include<hgl/filesystem/FileSystem.h>
#include<iostream>
#include"ShaderLib.h"
#include"GLSLCompiler.h"
#include<hgl/type/QTString.h>
#include"ConfigData.h"
#include"DLG_Config.h"

using namespace hgl;

int main(int argc,char **argv)
{
    QApplication qt_app(argc, argv);
    
    if(!glsl_compiler::Init())
    {        
        QMessageBox::warning(nullptr,"Fatal Error","Init GLSLCompiler failed!",QMessageBox::StandardButton::Abort);
        return -1;
    }

    if(!LoadConfig())
    {
        DLGConfig dlg;

        dlg.show();
        qt_app.exec();
    }

    OSString shader_lib_path=GetShaderLibraryPath();

    if(!shader_lib::LoadFromFolder(shader_lib_path))
    {
        os_err<<OS_TEXT("Load ShaderLib failed!")<<std::endl;
        return(-2);
    }
    
    qt_app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    qt_app.setAttribute(Qt::AA_EnableHighDpiScaling);
    qt_app.setFont(GetUIFont());

    MainWindow win;

    win.Init();
    
    win.setGeometry(QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        win.size(),
        qt_app.desktop()->availableGeometry()
    ));

    win.show();

    qt_app.exec();
    glsl_compiler::Close();
    return 0;
}
