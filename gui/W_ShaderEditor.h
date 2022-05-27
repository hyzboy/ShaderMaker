#pragma once

#include"W_EditorWidget.h"
#include"GLSLTextEdit.h"
#include<QLabel>
#include<QPushButton>
#include"WI_EditorTreeWidgetItem.h"

using namespace hgl;

class ShaderEditorWidget:public EditorWidget
{
    Q_OBJECT;

private:    //shader�༭��
    
    GLSLTextEdit *  glsl_editor;
    QLabel *        editor_hint;
    
    QPushButton *   save_button;

    void OnEditorCursorPositionChanged();
    void OnTextChanged();    
    void OnSave();

    QWidget *InitEditor(QWidget *);

private:    //shader�ɷ�Ԥ����
public:

    ShaderEditorWidget(EditorTreeWidgetItem *);
    virtual ~ShaderEditorWidget()=default;

    bool OnCloseRequested() override;    
};//class ShaderEditorWidget:public EditorWidget
