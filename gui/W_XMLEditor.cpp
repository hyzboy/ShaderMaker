#include"W_XMLEditor.h"
#include<QSplitter>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMessageBox>
#include<hgl/type/LoadStringList.h>
#include<hgl/type/MergeString.h>
#include<hgl/type/QTString.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/io/MemoryInputStream.h>
#include"QStringInfoOutput.h"
#include"ShaderLib.h"
#include"TypeDefine.h"
#include"GLSLCompiler.h"

QWidget *XMLEditorWidget::InitEditor(QWidget *parent)
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
        connect(save_button,&QPushButton::clicked,this,&XMLEditorWidget::OnSave);
        toolbar_layout->addWidget(save_button,0,Qt::AlignLeft);

        toolbar_layout->addStretch();

        editor_hint=new QLabel(toolbar);
        toolbar_layout->addWidget(editor_hint,0,Qt::AlignRight);

        toolbar->setFixedHeight(toolbar->sizeHint().height());

        layout->addWidget(toolbar);
    }

    {
        xml_editor=new XMLTextEdit(widget);

        UTF8StringList sl;

        if(LoadStringListFromTextFile(sl,GetFilename())>0)
        {
            const UTF8String str=MergeString(sl,UTF8String("\n"));

            xml_editor->setText(ToQString(str));
        }

        connect(xml_editor,&QTextEdit::cursorPositionChanged,this,&XMLEditorWidget::OnEditorCursorPositionChanged);
        connect(xml_editor,&QTextEdit::textChanged,this,&XMLEditorWidget::OnTextChanged);

        xml_editor->setReadOnly(false);
        save_button->setEnabled(false);
    }

    layout->addWidget(xml_editor);

    return widget;
}

XMLEditorWidget::XMLEditorWidget(const QString &in,const OSString &fn):EditorWidget(in,fn,U8_TEXT("glsl"))
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Vertical,this);

    {
        QWidget *up_widget=new QWidget(splitter);
        QHBoxLayout *up_layout=new QHBoxLayout(up_widget);

        up_layout->setContentsMargins(0,0,0,0);

        up_layout->addWidget(InitEditor(up_widget));
    }

    {
        log_widget=new QPlainTextEdit(splitter);

        log_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
        log_widget->setFrameShape(QFrame::StyledPanel);
        log_widget->setReadOnly(true);
        log_widget->setLineWrapMode(QPlainTextEdit::NoWrap);
        log_widget->setTabStopDistance(4);
    }

    splitter->setStretchFactor(0,5);
    splitter->setStretchFactor(1,2);
    layout->addWidget(splitter);
}

void XMLEditorWidget::OnEditorCursorPositionChanged()
{
    const QTextCursor cursor=xml_editor->textCursor();

    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    xml_editor->setExtraSelections(extraSelections);

    editor_hint->setText("Ln: "+QString::number(cursor.blockNumber()+1)
                    +"    Ch: "+QString::number(cursor.columnNumber()+1)
                    +"    ");
}

void XMLEditorWidget::OnTextChanged()
{
    save_button->setEnabled(true);
}

void XMLEditorWidget::OnSave()
{
    if(!save_button->isEnabled())return;

    const QString text=xml_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    filesystem::SaveMemoryToFile(GetFilename(),u8text.c_str(),u8text.Length());

    save_button->setEnabled(false);
}

bool XMLEditorWidget::OnCloseRequested()
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
