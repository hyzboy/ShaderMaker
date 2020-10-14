#include"W_XMLShaderEditor.h"
#include<QSplitter>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QMessageBox>
#include<hgl/type/StringList.h>
#include<hgl/type/QTString.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/io/MemoryInputStream.h>
#include"QStringInfoOutput.h"
#include"ShaderLib.h"
#include"TypeDefine.h"
#include"GLSLCompiler.h"

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

        compile_button=new QPushButton(toolbar);
        compile_button->setText("Compile");
        connect(compile_button,&QPushButton::clicked,this,&XMLShaderEditorWidget::OnCompile);
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

        glsl_preview->setReadOnly(false);

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
    const QTextCursor cursor=glsl_preview->textCursor();

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

void AppendToLog(QPlainTextEdit *widget,const QString &name,const UTF8StringList &sl)
{
    widget->appendHtml("<p><b>"+name+"</b><ul>");

    for(const UTF8String *str:sl)
        widget->appendHtml("<li>"+ToQString(*str)+"</li>");
    
    widget->appendHtml("</ul></p>");
}

void XMLShaderEditorWidget::OnCompile()
{
    glsl_preview->clear();
    log_widget->clear();
    
    const QString text=glsl_editor->toPlainText();

    const UTF8String u8text=ToUTF8String(text);

    io::MemoryInputStream mis;

    mis.Link(u8text.c_str(),u8text.Length());

    QString log;
    AutoDelete<InfoOutput> info_output=CreateQStringInfoOutput(&log);
    shader_lib::XMLShader *xs=shader_lib::LoadXMLShader(&mis,info_output);

    if(!xs)
    {
        log_widget->appendHtml("<p><font color=\"red\">Load XML Shader failed.</font></p>");

        log_widget->appendPlainText(log);
        return;
    }

    log_widget->appendHtml("<p><font color=\"darkGreen\">Load XML Shader OK.</font></p>");

    if(xs->in           .GetCount()>0)AppendToLog(log_widget,"Input Stages" ,xs->in);
    if(xs->out          .GetCount()>0)AppendToLog(log_widget,"Output Stages",xs->out);
    if(xs->struct_block .GetCount()>0)AppendToLog(log_widget,"Struct"       ,xs->struct_block);
    if(xs->raw          .GetCount()>0)AppendToLog(log_widget,"Raw"          ,xs->raw);
    if(xs->modules      .GetCount()>0)AppendToLog(log_widget,"Module"       ,xs->modules);

    if(xs->geom.max_vertices>0)
    {
        log_widget->appendHtml("<p><b>Geometry attributes</b><ul>");
        log_widget->appendHtml("<li><font color=\"blue\">in: </font>"+ToQString(xs->geom.in));
        log_widget->appendHtml("</li><li><font color=\"blue\">out: </font>"+ToQString(xs->geom.out));
        log_widget->appendHtml("</li><li><font color=\"blue\">max vertices: </font>"+QString::number(xs->geom.max_vertices));
        log_widget->appendHtml("</li></p>");
    }
    
    log.clear();
    
    xs->ext_name=ToUTF8String(GetItem()->text(ML_COLUMN_TYPE));

    shader_lib::XMLShaderMaker(xs,info_output);
    
    glsl_preview->setText(ToQString(xs->shader_source));
    log_widget->appendHtml(log);

    delete xs;
}
