#pragma once

#include<QWidget>
#include<QTreeWidget>
#include<QPushButton>
#include<QLabel>
#include<QHBoxLayout>
#include"GLSLTextEdit.h"
#include<hgl/type/String.h>
#include"InfoOutput.h"

#define ML_TYPE_MATERIAL   "Material"
#define ML_TYPE_VERTEX     "Vertex"
#define ML_TYPE_GEOMETRY   "Geometry"
#define ML_TYPE_FRAGMENT   "Fragment"

#define ML_COLUMN_NAME     0
#define ML_COLUMN_TYPE     1
#define ML_COLUMN_FILENAME 2

class TPMaterialLibrary:public QWidget
{
    Q_OBJECT

private:

    QTreeWidget *file_tree_widget;
    QTreeWidgetItem *current_item;

    void UpdateFileTree();
    void OnFileChanged(QTreeWidgetItem *,int);

    void InitEditor(QWidget *);

    InfoOutput *info_output;
    
    QWidget *build_widget;
    QHBoxLayout *build_layout;

    GLSLTextEdit *glsl_editor;
    QLabel *editor_hint;
    QTextEdit *log_text;
    QPushButton *save_button;
    QPushButton *convert_to_glsl_button;
    QPushButton *convert_and_compile_button;
    QPushButton *build_button;

    QPushButton *CreateButton(const QString &name,void (TPMaterialLibrary::*slot)());
    void OnCursorPositionChanged();

    hgl::OSString filename;
    bool text_changed;
    void OnTextChanged();

    void OnSave();
    void OnConvertToGLSL();
    void OnConvertAndCompile();
    void OnBuild();

public:

    TPMaterialLibrary();
    ~TPMaterialLibrary()=default;
};//