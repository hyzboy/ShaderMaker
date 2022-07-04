#include"TP_ShaderFile.h"
#include<QMenu>
#include"DLG_New.h"
#include<hgl/filesystem/FileSystem.h>

using namespace hgl;

class DLGNewShader:public DLGNew
{
    OSString base_path;

public:

    DLGNewShader(const OSString &path):DLGNew("shader",".glsl")
    {
        base_path=path;
    }

    ~DLGNewShader()=default;

    void OnOKClicked() override
    {
        close();
    }
};

void TPShaderFile::CreatePopupMenu()
{
    popup_menu=new QMenu(this);

    popup_menu->addAction(tr("&New shader"),this,&TPShaderFile::OnNew);
    popup_menu->addAction(tr("New &Folder"),this,&TPShaderFile::OnNewFolder);
    popup_menu->addSeparator();
    popup_menu->addAction(tr("&Rename"),this,&TPShaderFile::OnRename);
    popup_menu->addAction(tr("&Delete"),this,&TPShaderFile::OnDelete);
}

void TPShaderFile::OnPopupMenu(const QPoint &pos)
{
    popup_menu->popup(file_tree_widget->viewport()->mapToGlobal(pos));
}

void TPShaderFile::OnNew()
{
    DLGNew *dlg=new DLGNewShader(OS_TEXT(""));

    dlg->show();
}

void TPShaderFile::OnNewFolder()
{
    //DLGNew *dlg=new DLGNew(tr("folder"),"");

    //dlg->show();
}

void TPShaderFile::OnRename()
{
}

void TPShaderFile::OnDelete()
{
}
