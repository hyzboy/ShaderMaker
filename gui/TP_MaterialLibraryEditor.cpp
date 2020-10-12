#include"TP_MaterialLibrary.h"
#include<QSPlitter>
#include<QHBoxLayout>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/QTString.h>

using namespace hgl;
using namespace hgl::filesystem;

class QTInfoOutput:public InfoOutput
{
};//

void TPMaterialLibrary::InitEditor(QWidget *parent)
{
    QSplitter *right_splitter=new QSplitter(Qt::Vertical,parent);

    {
        glsl_editor=new GLSLTextEdit(right_splitter);

        glsl_editor->setReadOnly(false);

        connect(glsl_editor,&QTextEdit::cursorPositionChanged,this,&TPMaterialLibrary::OnCursorPositionChanged);
        connect(glsl_editor,&QTextEdit::textChanged,this,&TPMaterialLibrary::OnTextChanged);
    }

    {
        build_widget=new QWidget(right_splitter);
        build_layout=new QHBoxLayout(build_widget);

        build_layout->setContentsMargins(0,0,0,0);

        {
            save_button                 =CreateButton("Save",               &TPMaterialLibrary::OnSave);
            convert_to_glsl_button      =CreateButton("Convert to GLSL",    &TPMaterialLibrary::OnConvertToGLSL);
            convert_and_compile_button  =CreateButton("Convert&&Compile",   &TPMaterialLibrary::OnConvertAndCompile);
            build_button                =CreateButton("Build",              &TPMaterialLibrary::OnBuild);

            build_layout->addStretch();

            editor_hint=new QLabel(build_widget);
            build_layout->addWidget(editor_hint,0,Qt::AlignRight);
        }

        build_widget->setFixedHeight(build_widget->height());
    }

    {
        log_text=new QTextEdit(right_splitter);

        log_text->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
        log_text->setFrameShape(QFrame::StyledPanel);
        log_text->setReadOnly(true);
        log_text->setLineWrapMode(QTextEdit::NoWrap);
        log_text->setTabStopWidth(4);
    }

    right_splitter->setStretchFactor(0,5);
    right_splitter->setStretchFactor(1,0);
    right_splitter->setStretchFactor(2,2);
}

QPushButton *TPMaterialLibrary::CreateButton(const QString &name,void (TPMaterialLibrary::*func)())
{
    QPushButton *button=new QPushButton(name,build_widget);
    build_layout->addWidget(button,0,Qt::AlignLeft);
    connect(button,&QPushButton::clicked,this,func);
    button->setHidden(true);

    return button;
}

void TPMaterialLibrary::OnCursorPositionChanged()
{
    const QTextCursor cursor=glsl_editor->textCursor();

    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    glsl_editor->setExtraSelections(extraSelections);

    editor_hint->setText("Ln: "+QString::number(cursor.blockNumber()+1)
                    +"    Ch: "+QString::number(cursor.columnNumber()+1)
                    +"    ");
}

void TPMaterialLibrary::OnTextChanged()
{
    text_changed=true;

    save_button->setEnabled(true);
    save_button->setHidden(false);
}

void TPMaterialLibrary::OnSave()
{
    if(!text_changed)return;

    const QString text=glsl_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    SaveMemoryToFile(filename,u8text.c_str(),u8text.Length());

    text_changed=false;
    save_button->setEnabled(false);
    save_button->setHidden(true);
}