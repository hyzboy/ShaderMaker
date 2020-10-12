#include"TP_Varying.h"
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<QVBoxLayout>
#include<QSplitter>
#include<QListWidget>

QSyntaxHighlighter *CreateGLSLSyntaxHighlighter(QTextDocument *document);

TPVarying::TPVarying()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Horizontal,this);
    
    layout->setContentsMargins(QMargins(0,0,0,0));

    //左侧，Varying列表
    {
        QListWidget *list=new QListWidget(splitter);
    
        list->setFrameShape(QFrame::StyledPanel);
        list->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

        for(const hgl::UTF8String *str:shader_lib::GetVaryingList())
            list->addItem(toQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPVarying::OnVaryingClicked);
    }

    //右侧，预览区
    {
        sl_preview=new QTextEdit(splitter);

        sl_preview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
        sl_preview->setFrameShape(QFrame::StyledPanel);
        sl_preview->setReadOnly(true);
        sl_preview->setLineWrapMode(QTextEdit::NoWrap);
        sl_preview->setTabStopWidth(4);
        {
            QFont font=sl_preview->font();

            font.setFixedPitch(true);
            font.setStyleHint(QFont::Monospace);
            font.setFamily("Consolas");

            sl_preview->setFont(font);
        }

        highlighter=CreateGLSLSyntaxHighlighter(sl_preview->document());
    }

    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,4);
    
    layout->addWidget(splitter);
}

TPVarying::~TPVarying()
{
}

void TPVarying::OnVaryingClicked(QListWidgetItem *item)
{
    sl_preview->clear();

    const shader_lib::VaryingConfig *vc=shader_lib::GetVarying(toUTF8String(item->text()));

    QString str;

    for(const shader_lib::Varying *v:*vc)
    {
        str.append(toQString(v->type));
        str.append(" ");
        str.append(toQString(v->name));
        str.append(";");

        if(!v->comment.IsEmpty())
        {
            str.append("    //");
            str.append(toQString(v->comment));
        }

        str.append("\n");
    }

    sl_preview->setPlainText(str);
}
