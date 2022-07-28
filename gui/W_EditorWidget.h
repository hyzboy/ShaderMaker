#pragma once

#include<QWidget>
#include<hgl/type/String.h>

using namespace hgl;

class EditorWidget:public QWidget
{
    Q_OBJECT

private:

    QString item_name;
    OSString filename;
    UTF8String type_name;

public:

    const QString & GetItemName()const{return item_name;}
    const OSString &GetFilename()const{return filename;}
    const UTF8String &GetTypename()const{return type_name;}

public:

    EditorWidget(const QString &in,const OSString &fn,const UTF8String &tn)
    {
        item_name=in;
        filename=fn;
        type_name=tn;
    }

    virtual ~EditorWidget()=default;

    virtual bool OnCloseRequested()=0;
};//class EditorWidget:public QWidget
