#pragma once

#include<QWidget>
#include<hgl/type/String.h>
#include"WI_EditorTreeWidgetItem.h"

using namespace hgl;

class EditorWidget:public QWidget
{
    Q_OBJECT

private:

    EditorTreeWidgetItem *item;

public:

    const EditorTreeWidgetItem *GetItem     ()const{return item;}
    const os_char *             GetFilename ()const{return item->GetFilename();}

public:

    EditorWidget(EditorTreeWidgetItem *i)
    {
        item=i;
    }

    virtual ~EditorWidget()=default;

    virtual bool OnCloseRequested()=0;
};//class EditorWidget:public QWidget
