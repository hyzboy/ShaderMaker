#pragma once

#include"W_EditorWidget.h"
#include<QLabel>
#include<hgl/type/String.h>
#include"ShaderLib.h"
#include"GLSLTextEdit.h"
#include<QPushButton>
#include<QPlainTextEdit>

using namespace hgl;

class MaterialEditorWidget:public EditorWidget
{
    Q_OBJECT

private:

    GLSLTextEdit *glsl_editor;
    QLabel *editor_hint;

    QPushButton *save_button,
                *compile_button,
                *preview_button;

    void OnEditorCursorPositionChanged();
    void OnTextChanged();

    void OnSave();
    void OnCompile();
    void OnPreview();

    QWidget *InitEditor(QWidget *);
    
private:    //日志区

    QPlainTextEdit *log_widget;

public:

    MaterialEditorWidget(QTreeWidgetItem *,const OSString &);
    virtual ~MaterialEditorWidget()=default;

    bool OnCloseRequested() override;
};//