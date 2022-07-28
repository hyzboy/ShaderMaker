#pragma once

#include"W_EditorWidget.h"
#include<QLabel>
#include<QPushButton>
#include<QPlainTextEdit>
#include"GLSLTextEdit.h"
#include<hgl/type/String.h>

using namespace hgl;

class XMLShaderEditorWidget:public EditorWidget
{
    Q_OBJECT

private:    //XMLShader编辑区

    GLSLTextEdit *glsl_editor;
    QLabel *editor_hint;

    QPushButton *save_button,
                *compile_button;

    void OnEditorCursorPositionChanged();
    void OnTextChanged();

    void OnSave();
    void OnCompile();

    QWidget *InitEditor(QWidget *);

private:    //glsl预览区

    GLSLTextEdit *glsl_preview;
    QLabel *preview_hint;

    QPushButton *save_glsl_button;
    
    void OnPreviewCursorPositionChanged();

    QWidget *InitPreview(QWidget *);

private:    //日志区

    QPlainTextEdit *log_widget;

public:

    XMLShaderEditorWidget(const QString &,const OSString &);
    virtual ~XMLShaderEditorWidget()=default;

    bool OnCloseRequested() override;
};//