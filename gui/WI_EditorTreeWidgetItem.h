#pragma once
#include<QTreeWidgetItem>
#include<hgl/type/String.h>
#include<hgl/filesystem/FileSystem.h>

using namespace hgl;
using namespace hgl::filesystem;

class EditorTreeWidgetItem:public QTreeWidgetItem
{
    OSString fullname;
    bool is_folder;

public:

    EditorTreeWidgetItem(QTreeWidgetItem *parent,const QStringList &string_list,const FileInfo *fi):
        QTreeWidgetItem(parent,string_list)
    {
        if(fi)
        {
            fullname=fi->fullname;
            is_folder=fi->is_directory;
        }
        else
        {
            is_folder=true;
        }
    }

    const QString GetName()const
    {
        return text(0);
    }

    const bool isFolder()const
    {
        return is_folder;
    }

    const OSString &GetFilename()const
    {
        return fullname;
    }
};