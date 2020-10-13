#pragma once

#include<QWidget>
#include<QTreeWidgetItem>
#include<hgl/type/String.h>

using namespace hgl;

class EditorWidget:public QWidget
{
    Q_OBJECT

private:

    QTreeWidgetItem *item;

    OSString filename;

public:

    const QTreeWidgetItem *GetItem()const{return item;}
    const OSString &GetFilename()const{return filename;}

public:

    EditorWidget(QTreeWidgetItem *i,const OSString &fn)
    {
        item=i;
        filename=fn;
    }

    virtual ~EditorWidget()=default;

    virtual bool OnCloseRequested()=0;
};//class EditorWidget:public QWidget
