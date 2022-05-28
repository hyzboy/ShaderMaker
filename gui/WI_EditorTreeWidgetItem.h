#pragma once
#include<QTreeWidgetItem>
#include<hgl/type/String.h>
#include<hgl/filesystem/FileSystem.h>
#include"TypeDefine.h"

using namespace hgl;
using namespace hgl::filesystem;

class EditorTreeWidgetItem:public QTreeWidgetItem
{
    FileInfo file_info;

    MaterialFileType mft;

public:

    EditorTreeWidgetItem(QTreeWidgetItem *          parent,
                         const QStringList &        string_list,
                         const FileInfo *           fi          =nullptr,
                         const MaterialFileType &   t           =MaterialFileType::None):
        QTreeWidgetItem(parent,string_list)
    {
        if(fi)
        {
            hgl_cpy(file_info,*fi);
        }
        else
        {
            hgl_zero(file_info);
        }

        mft=t;
    }

    const QString GetName()const
    {
        return text(0);
    }

    const bool isFolder()const
    {
        return file_info.is_directory;
    }

    const os_char *GetFilename()const
    {
        return file_info.fullname;
    }

    const MaterialFileType &type()const{return mft;}

    const char *GetTypename()const
    {
        return MaterialFileTypeName[(size_t)mft];
    }
};
