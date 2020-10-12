#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include"GLSLTextEdit.h"

class TPModule:public QWidget
{
    Q_OBJECT

private:

    void OnModuleClicked(QListWidgetItem *item);

private:

    QListWidget *depend_raw_list;
    QListWidget *depend_struct_list;

    QListWidget *CreateDependGroup(QLayout *,const QString &);
    
    GLSLTextEdit *glsl_editor;

public:

    TPModule();
    ~TPModule();
};
