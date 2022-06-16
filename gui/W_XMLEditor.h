#pragma once

#include"W_EditorWidget.h"
#include<QLabel>
#include<QPushButton>
#include<QPlainTextEdit>
#include"XMLTextEdit.h"
#include<hgl/type/String.h>

using namespace hgl;

class XMLEditorWidget:public EditorWidget
{
    Q_OBJECT

private:    //XMLShader编辑区

    XMLTextEdit *xml_editor;
    QLabel *editor_hint;

    QPushButton *save_button;

    void OnEditorCursorPositionChanged();
    void OnTextChanged();

    void OnSave();

    QWidget *InitEditor(QWidget *);

private:    //日志区

    QPlainTextEdit *log_widget;

public:

    XMLEditorWidget(EditorTreeWidgetItem *);
    virtual ~XMLEditorWidget()=default;

    bool OnCloseRequested() override;
};//