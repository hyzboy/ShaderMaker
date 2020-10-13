#include"W_XMLShaderEditor.h"
#include<QSplitter>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMessageBox>
#include<hgl/type/StringList.h>
#include<hgl/type/QTString.h>
#include<hgl/filesystem/FileSystem.h>

QWidget *XMLShaderEditorWidget::InitEditor(QWidget *parent)
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
        connect(save_button,&QPushButton::clicked,this,&XMLShaderEditorWidget::OnSave);
        toolbar_layout->addWidget(save_button,0,Qt::AlignLeft);

        convert_button=new QPushButton(toolbar);
        convert_button->setText("Convert to GLSL");
        toolbar_layout->addWidget(convert_button,0,Qt::AlignLeft);

        compile_button=new QPushButton(toolbar);
        compile_button->setText("Compile");
        toolbar_layout->addWidget(compile_button,0,Qt::AlignLeft);

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

        connect(glsl_editor,&QTextEdit::cursorPositionChanged,this,&XMLShaderEditorWidget::OnEditorCursorPositionChanged);
        connect(glsl_editor,&QTextEdit::textChanged,this,&XMLShaderEditorWidget::OnTextChanged);

        glsl_editor->setReadOnly(false);
        save_button->setEnabled(false);
    }

    layout->addWidget(glsl_editor);

    return widget;
}

QWidget *XMLShaderEditorWidget::InitPreview(QWidget *parent)
{
    QWidget *widget=new QWidget(parent);
    
    QVBoxLayout *layout=new QVBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    {
        QWidget *toolbar=new QWidget(widget);
        QHBoxLayout *toolbar_layout=new QHBoxLayout(toolbar);

        toolbar_layout->setContentsMargins(0,0,0,0);

        save_glsl_button=new QPushButton(toolbar);
        save_glsl_button->setText("Save To ...");
        toolbar_layout->addWidget(save_glsl_button,0,Qt::AlignLeft);

        toolbar_layout->addStretch();

        preview_hint=new QLabel(toolbar);
        toolbar_layout->addWidget(preview_hint,0,Qt::AlignRight);

        toolbar->setFixedHeight(toolbar->sizeHint().height());

        layout->addWidget(toolbar);
    }

    {
        glsl_preview=new GLSLTextEdit(widget);

        connect(glsl_preview,&QTextEdit::cursorPositionChanged,this,&XMLShaderEditorWidget::OnPreviewCursorPositionChanged);
    }

    layout->addWidget(glsl_preview);

    return widget;
}

XMLShaderEditorWidget::XMLShaderEditorWidget(QTreeWidgetItem *i,const OSString &fn):EditorWidget(i,fn)
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Vertical,this);

    {
        QWidget *up_widget=new QWidget(splitter);
        QHBoxLayout *up_layout=new QHBoxLayout(up_widget);

        up_layout->setContentsMargins(0,0,0,0);

        up_layout->addWidget(InitEditor(up_widget));
        up_layout->addWidget(InitPreview(up_widget));
    }

    {
        log_widget=new QPlainTextEdit(splitter);

        log_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        log_widget->setFrameShape(QFrame::StyledPanel);
        log_widget->setReadOnly(true);
        log_widget->setLineWrapMode(QPlainTextEdit::NoWrap);
        log_widget->setTabStopWidth(4);
    }

    splitter->setStretchFactor(0,5);
    splitter->setStretchFactor(1,2);
    layout->addWidget(splitter);
}

void XMLShaderEditorWidget::OnEditorCursorPositionChanged()
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

void XMLShaderEditorWidget::OnPreviewCursorPositionChanged()
{
    const QTextCursor cursor=glsl_editor->textCursor();

    preview_hint->setText("Ln: "+QString::number(cursor.blockNumber()+1)
                     +"    Ch: "+QString::number(cursor.columnNumber()+1)
                     +"    ");
}

void XMLShaderEditorWidget::OnTextChanged()
{
    save_button->setEnabled(true);
}

void XMLShaderEditorWidget::OnSave()
{
    if(!save_button->isEnabled())return;

    const QString text=glsl_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    filesystem::SaveMemoryToFile(GetFilename(),u8text.c_str(),u8text.Length());

    save_button->setEnabled(false);
}

bool XMLShaderEditorWidget::OnCloseRequested()
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