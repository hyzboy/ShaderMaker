#pragma once

#include"W_EditorWidget.h"
#include<QLabel>
#include<QPushButton>
#include<QCheckBox>
#include<QLineEdit>
#include<QPlainTextEdit>
#include<hgl/type/String.h>
#include"ShaderLib.h"

using namespace hgl;

class MaterialEditorWidget:public EditorWidget
{
    Q_OBJECT

private:

    QCheckBox *tess_check;
    QCheckBox *geom_check;

    struct ShaderEditArea
    {
        QLabel *name_label;
        QLineEdit *fn_edit;
        QPushButton *file_browser;
        QPushButton *open_file;

    public:

        void SetEnable(bool open)
        {
            name_label->setEnabled(open);
            fn_edit->setEnabled(open);
            file_browser->setEnabled(open);
            open_file->setEnabled(open);
        }

        void Enable()
        {
            SetEnable(true);
        }

        void Disable()
        {
            SetEnable(false);
        }
    };

    ShaderEditArea sea[5];

    QPushButton *build;
    QPushButton *preview;

    QPlainTextEdit *log_widget;

public:

    MaterialEditorWidget(QTreeWidgetItem *,const OSString &);
    virtual ~MaterialEditorWidget()=default;

    bool OnCloseRequested() override;
};//