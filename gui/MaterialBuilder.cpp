#include<QApplication>
#include<QFont>
#include<QStyle>
#include<QDesktopWidget>
#include<QMessageBox>
#include"MainWindow.h"
#include<hgl/filesystem/FileSystem.h>
#include<iostream>
#include"ShaderLib.h"
#include<hgl/type/QTString.h>

using namespace hgl;

bool InitShaderLibPath(OSString &path)
{
    OSString cur_path;
    OSString filename;
    
    if(!filesystem::GetLocalAppdataPath(cur_path))
    {
        QMessageBox::information(nullptr,"Fatal Error","Get path failed which Local AppData",QMessageBox::StandardButton::Abort);
        
        return(false);
    }

    filename=filesystem::MergeFilename(cur_path, OS_TEXT("shader_libs.config"));

    if(!filesystem::FileExist(filename))
    {
        QMessageBox::information(nullptr, "Warning", toQString(OS_TEXT("We don't find the config file, please select a new folder.\nThe config file should use filename: \n\n\t")+filename), QMessageBox::StandardButton::Ok);

        return(false);
    }

    return(true);
}

int main(int argc,char **argv)
{
    QApplication qt_app(argc, argv);

    OSString cfg_path;

    if(InitShaderLibPath(cfg_path))
    {
    }
    else
    {
    }

    OSString cur_path;

    if(!filesystem::GetCurrentPath(cur_path))
    {
        os_err<<OS_TEXT("GetCurrentPath failed!")<<std::endl;
        return(-1);
    }

    const OSString shader_lib_path=filesystem::MergeFilename(cur_path,OS_TEXT("shader_libs"));

    if(!shader_lib::LoadFromFolder(shader_lib_path))
    {
        os_err<<OS_TEXT("Load ShaderLib failed!")<<std::endl;
        return(-2);
    }

    
    qt_app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    {
        QFont fnt=qt_app.font();
        const int fs=fnt.pointSize();
        if(fs<9)
            fnt.setPointSize(9);

        qt_app.setFont(fnt);
    }

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
    return 0;
}
