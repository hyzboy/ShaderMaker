#include<QApplication>
#include<QFont>
#include<QStyle>
#include<QDesktopWidget>
#include"MainWindow.h"
#include<hgl/filesystem/FileSystem.h>
#include<iostream>
#include"ShaderLib.h"

using namespace hgl;

int main(int argc,char **argv)
{
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

    QApplication qt_app(argc,argv);
    
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
