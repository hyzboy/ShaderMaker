#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include"GLSLTextEdit.h"

class TPVarying:public QWidget
{
    Q_OBJECT

private:

    void OnVaryingClicked(QListWidgetItem *item);

private:

    GLSLTextEdit *glsl_editor;

public:

    TPVarying();
    ~TPVarying();
};
