#pragma once

#include<QWidget>
#include<QTreeWidget>
#include"GLSLTextEdit.h"

enum class ShaderFileColumn
{
    Name=0,
    Filename
};

class TPShaderFile:public QWidget
{
    Q_OBJECT

private:
    
    QTreeWidget *file_tree_widget;
    QTreeWidgetItem *current_item;

    void UpdateFileTree();
    void OnFileChanged(QTreeWidgetItem *,int);

private:

    GLSLTextEdit *glsl_editor;

public:

    TPShaderFile();
};
