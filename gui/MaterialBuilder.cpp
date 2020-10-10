#include<QApplication>
#include<QFont>
#include<QStyle>
#include<QDesktopWidget>
#include<QMessageBox>
#include<QDir>
#include<QFileDialog>
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
        QMessageBox::warning(nullptr,"Fatal Error","Get path failed which Local AppData",QMessageBox::StandardButton::Abort);
        
        return(false);
    }

    filename=filesystem::MergeFilename(cur_path,OS_TEXT("shader_libs.config"));

    if(!filesystem::FileExist(filename))
    {
        QMessageBox::warning(nullptr, "Warning", toQString(OS_TEXT("We don't find the config file, please select a new folder.\nThe config file should use filename: \n\n\t")+filename), QMessageBox::StandardButton::Ok);

        QString path_name=QFileDialog::getExistingDirectory(nullptr,"Select directory of ShaderLib",toQString(cur_path),QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

        if(path_name.size()<=0)
            return(false);

        path=toOSString(path_name);

        filesystem::SaveMemoryToFile(filename,path.c_str(),path.Length()*sizeof(os_char));
    }
    else
    {
        LoadStringFromTextFile(path,filename,OSCharSet);
    }

    return(true);
}

int main(int argc,char **argv)
{
    QApplication qt_app(argc, argv);

    OSString shader_lib_path;

    if(!InitShaderLibPath(shader_lib_path))
        return(-1);

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
