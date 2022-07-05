#include"TP_ShaderFile.h"
#include<QMenu>
#include"DLG_New.h"
#include<hgl/filesystem/FileSystem.h>

using namespace hgl;

class DLGNewShader:public DLGNameEdit
{
    OSString base_path;

public:

    DLGNewShader(const OSString &path):DLGNameEdit("shader",".glsl")
    {
        base_path=path;
    }

    ~DLGNewShader()=default;

    void OnOKClicked() override
    {
        close();
    }
};

class DLGRename:public DLGNameEdit
{
    OSString origin_name;

public:

    DLGRename(const OSString &name):DLGNameEdit("file",name)
    {
        origin_name=name;
    }

    ~DLGRename()=default;

    void OnOKClicked() override
    {
        close();
    }
};

void TPShaderFile::CreatePopupMenu()
{
    popup_menu_file=new QMenu(this);

    popup_menu_file->addAction(tr("&Rename"),this,&TPShaderFile::OnRename);
    popup_menu_file->addAction(tr("&Delete"),this,&TPShaderFile::OnDelete);

    popup_menu_folder=new QMenu(this);

    popup_menu_folder->addAction(tr("&New shader"),this,&TPShaderFile::OnNew);
    popup_menu_folder->addAction(tr("New &Folder"),this,&TPShaderFile::OnNewFolder);
    popup_menu_folder->addSeparator();
    popup_menu_folder->addAction(tr("&Rename"),this,&TPShaderFile::OnRename);
    popup_menu_folder->addAction(tr("&Delete"),this,&TPShaderFile::OnDelete);
}

void TPShaderFile::OnPopupMenu(const QPoint &pos)
{
    if(!current_item)return;

    QMenu *pm=(current_item->isFolder())?popup_menu_folder:popup_menu_file;

    pm->popup(file_tree_widget->viewport()->mapToGlobal(pos));
}

void TPShaderFile::OnNew()
{
    DLGNameEdit *dlg=new DLGNewShader(OS_TEXT(""));

    dlg->show();
}

void TPShaderFile::OnNewFolder()
{
    //DLGNameEdit *dlg=new DLGNameEdit(tr("folder"),"");

    //dlg->show();
}

void TPShaderFile::OnRename()
{
}

void TPShaderFile::OnDelete()
{
}
