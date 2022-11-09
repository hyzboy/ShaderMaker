#pragma once

#include<hgl/type/String.h>
#include<QDialog>
#include<QStringList>

class DLGGUIStyle:public QDialog
{
    Q_OBJECT

private:

    QStringList style_list;
    QString choose_style;

    void OnStyleChange(int);

    void OnFinished(int);

public:

    DLGGUIStyle();
    ~DLGGUIStyle()=default;
};//
