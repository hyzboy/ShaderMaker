#pragma once

#include<QWidget>
#include<QTreeWidget>
#include<QPushButton>
#include<QLabel>
#include"GLSLTextEdit.h"
#include<hgl/type/String.h>

#define COLUMN_NAME     0
#define COLUMN_TYPE     1
#define COLUMN_FILENAME 2

class TPMaterialLibrary:public QWidget
{
    Q_OBJECT

private:

    QTreeWidget *file_tree_widget;
    QTreeWidgetItem *current_item;

    void UpdateFileTree();
    void OnFileChanged(QTreeWidgetItem *,int);

    void InitEditor(QWidget *);

    GLSLTextEdit *glsl_editor;
    QLabel *editor_hint;
    QTextEdit *log_text;
    QPushButton *save_button;
    QPushButton *convert_to_glsl_button;
    QPushButton *convert_and_compile_button;
    QPushButton *build_button;

    void OnCursorPositionChanged();

    hgl::OSString filename;
    bool text_changed;
    void OnTextChanged();

    void OnSave();

public:

    TPMaterialLibrary();
    ~TPMaterialLibrary()=default;
};//