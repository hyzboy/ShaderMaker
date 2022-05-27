#include"W_ShaderEditor.h"
#include<QSplitter>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMessageBox>
#include<hgl/type/StringList.h>
#include<hgl/type/QTString.h>
#include<hgl/filesystem/FileSystem.h>

QWidget *ShaderEditorWidget::InitEditor(QWidget *parent)
{
    QWidget *widget=new QWidget(parent);
    
    QVBoxLayout *layout=new QVBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    {
        QWidget *toolbar=new QWidget(widget);
        QHBoxLayout *toolbar_layout=new QHBoxLayout(toolbar);

        toolbar_layout->setContentsMargins(0,0,0,0);

        save_button=new QPushButton(toolbar);
        save_button->setText("Save");
        connect(save_button,&QPushButton::clicked,this,&ShaderEditorWidget::OnSave);
        toolbar_layout->addWidget(save_button,0,Qt::AlignLeft);

        toolbar_layout->addStretch();

        editor_hint=new QLabel(toolbar);
        toolbar_layout->addWidget(editor_hint,0,Qt::AlignRight);

        toolbar->setFixedHeight(toolbar->sizeHint().height());

        layout->addWidget(toolbar);
    }

    {
        glsl_editor=new GLSLTextEdit(widget);

        UTF8StringList sl;

        if(LoadStringListFromTextFile(sl,GetFilename())>0)
        {
            const UTF8String str=ToString(sl,UTF8String("\n"));

            glsl_editor->setText(ToQString(str));
        }

        connect(glsl_editor,&QTextEdit::cursorPositionChanged,  this,&ShaderEditorWidget::OnEditorCursorPositionChanged);
        connect(glsl_editor,&QTextEdit::textChanged,            this,&ShaderEditorWidget::OnTextChanged);

        glsl_editor->setReadOnly(false);
        save_button->setEnabled(false);
    }

    layout->addWidget(glsl_editor);

    return widget;
}

ShaderEditorWidget::ShaderEditorWidget(EditorTreeWidgetItem *i):EditorWidget(i)
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    
    QWidget *up_widget=new QWidget(this);
    {
        QHBoxLayout *up_layout=new QHBoxLayout(up_widget);

        up_layout->setContentsMargins(0,0,0,0);

        up_layout->addWidget(InitEditor(up_widget));
//        up_layout->addWidget(InitPreview(up_widget));
    }

    layout->addWidget(up_widget);
}

void ShaderEditorWidget::OnEditorCursorPositionChanged()
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

void ShaderEditorWidget::OnTextChanged()
{
    save_button->setEnabled(true);
}

void ShaderEditorWidget::OnSave()
{
    if(!save_button->isEnabled())return;

    const QString text=glsl_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    filesystem::SaveMemoryToFile(GetFilename(),u8text.c_str(),u8text.Length());

    save_button->setEnabled(false);
}

bool ShaderEditorWidget::OnCloseRequested()
{
    if(!save_button->isEnabled())return(true);

    QMessageBox msgBox;
    
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if(ret==QMessageBox::Save)
    {
        OnSave();
        return(true);
    }
    else
    if(ret==QMessageBox::Discard)
    {
        return(true);
    }
    else
        return(false);
}