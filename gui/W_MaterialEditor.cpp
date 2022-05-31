#include"W_MaterialEditor.h"
#include<QVBoxLayout>
#include<QSplitter>
#include<QMessageBox>
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/filesystem/FileSystem.h>
#include"QStringInfoOutput.h"
#include"ConfigData.h"

using namespace shader_lib;

QWidget *MaterialEditorWidget::InitEditor(QWidget *parent)
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
        connect(save_button,&QPushButton::clicked,this,&MaterialEditorWidget::OnSave);
        toolbar_layout->addWidget(save_button,0,Qt::AlignLeft);

        compile_button=new QPushButton(toolbar);
        compile_button->setText("Compile");
        connect(compile_button,&QPushButton::clicked,this,&MaterialEditorWidget::OnCompile);
        toolbar_layout->addWidget(compile_button,0,Qt::AlignLeft);

        preview_button=new QPushButton(toolbar);
        preview_button->setText("Preview");
        preview_button->setDisabled(true);
        toolbar_layout->addWidget(preview_button,0,Qt::AlignLeft);

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

        connect(glsl_editor,&QTextEdit::cursorPositionChanged,this,&MaterialEditorWidget::OnEditorCursorPositionChanged);
        connect(glsl_editor,&QTextEdit::textChanged,this,&MaterialEditorWidget::OnTextChanged);

        glsl_editor->setReadOnly(false);
        save_button->setEnabled(false);
    }

    layout->addWidget(glsl_editor);

    return widget;
}

MaterialEditorWidget::MaterialEditorWidget(EditorTreeWidgetItem *i):EditorWidget(i)
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

        log_widget->setTabStopDistance(4);      //qt6
        //log_widget->setTabStopWidth(4);       //qt5
    }

    splitter->setStretchFactor(0,2);
    splitter->setStretchFactor(1,5);
    layout->addWidget(splitter);
}

void MaterialEditorWidget::OnEditorCursorPositionChanged()
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

void MaterialEditorWidget::OnTextChanged()
{
    save_button->setEnabled(true);
}

void MaterialEditorWidget::OnSave()
{
    if(!save_button->isEnabled())return;

    const QString text=glsl_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    filesystem::SaveMemoryToFile(GetFilename(),u8text.c_str(),u8text.Length());

    save_button->setEnabled(false);
}

bool MaterialEditorWidget::OnCloseRequested()
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

void MaterialEditorWidget::OnCompile()
{
    log_widget->clear();
    
    if(save_button->isEnabled())
    {
        QMessageBox msgBox;
    
        msgBox.setText("The document has been modified. You need to save before compile.");
        msgBox.setInformativeText("Do you want to save your changes and compile material ?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        
        if(msgBox.exec()!=QMessageBox::Save)
            return;
    }

    OnSave();
    
    QString log;
    AutoDelete<InfoOutput> info_output=CreateQStringInfoOutput(&log);

    shader_lib::XMLMaterial *mat=shader_lib::LoadXMLMaterial(GetFilename(),info_output);

    log_widget->appendHtml(log);
    log.clear();

    if(mat)
    {
        info_output->colorWrite("green","<p>Make material data OK!</p>");

        const OSString output_path=GetMaterialOutputPath();
        
        const OSString short_filename=filesystem::ClipFilename(OSString(GetFilename()));

        const OSString mat_filename=filesystem::TrimFileExtName(short_filename,true);

        const OSString output_filename=filesystem::MergeFilename(output_path,mat_filename);

        shader_lib::SaveMaterial(output_filename,mat,info_output);

        delete mat;
    }
    
    log_widget->appendHtml(log);
}
