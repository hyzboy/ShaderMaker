#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include"GLSLTextEdit.h"

class TPRawGLSL:public QWidget
{
    Q_OBJECT

private:

    void OnRawGLSLClicked(QListWidgetItem *item);

private:

    GLSLTextEdit *glsl_editor;

public:

    TPRawGLSL();
    ~TPRawGLSL();
};
