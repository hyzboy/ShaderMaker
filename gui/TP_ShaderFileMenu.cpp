#include"TP_ShaderFile.h"
#include<QMenu>
#include<hgl/qt/dialog/NameEdit.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/QTString.h>

using namespace hgl;
using namespace hgl::qt;

class DLGNewFile:public dialog::NameEdit
{
    EditorTreeWidgetItem *tree_item;

public:

    DLGNewFile(EditorTreeWidgetItem *item,const QString &file_type_name,const QString &default_name)
        :dialog::NameEdit("Please enter a name for the "+file_type_name,default_name)
    {
        tree_item=item;
    }

    ~DLGNewFile()=default;

    bool OnNameCheck(QString &err_text) override
    {
        const OSString &filename=ToOSString(name_edit->text());
        const OSString &fullname=filesystem::MergeFilename(tree_item->GetFilename(),filename);

        if(filesystem::FileExist(fullname))
        {
            err_text="the file\n\""+ToQString(fullname)+"\"\nalready exists, please input a new name.";
            return(false);
        }

        const UTF8String str="// glsl: "+to_u8(fullname)+"\n";

        filesystem::SaveMemoryToFile(fullname,str.c_str(),str.Length());

        {
            filesystem::FileInfo fi;
            filesystem::GetFileInfo(fullname,fi);
            
            const OSString s_fn=ClipFileMainname(filename);

            hgl::strcpy(fi.name,sizeof(fi.name)/sizeof(os_char),filename.c_str());
            hgl::strcpy(fi.fullname,sizeof(fi.fullname)/sizeof(os_char),fullname.c_str());

            tree_item->addChild(new EditorTreeWidgetItem(tree_item,QStringList(ToQString(s_fn)),&fi));
        }

        return(true);
    }
};

class DLGNewGLSL:public DLGNewFile
{
public:

    DLGNewGLSL(EditorTreeWidgetItem *tree_item):DLGNewFile(tree_item,"GLSL",".glsl")
    {
    }
};

class DLGRename:public dialog::NameEdit
{
    OSString origin_name;

public:

    DLGRename(const OSString &name):dialog::NameEdit("file",ToQString(name))
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

    popup_menu_folder->addAction(tr("&New shader"),this,&TPShaderFile::OnNewGLSL);
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

void TPShaderFile::OnNewGLSL()
{
    if(!current_item)return;
    if(!current_item->isFolder())return;

    DLGNewGLSL *dlg=new DLGNewGLSL(current_item);
    
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
