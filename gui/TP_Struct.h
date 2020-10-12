#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include"GLSLTextEdit.h"

class TPStruct:public QWidget
{
    Q_OBJECT

private:

    void OnStructClicked(QListWidgetItem *item);

private:

    GLSLTextEdit *glsl_editor;

public:

    TPStruct();
    ~TPStruct();
};
