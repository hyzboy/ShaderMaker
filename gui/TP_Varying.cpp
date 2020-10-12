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
            list->addItem(ToQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPVarying::OnVaryingClicked);
    }

    //右侧，预览区
    {
        glsl_editor=new GLSLTextEdit(splitter);
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
    glsl_editor->clear();

    const shader_lib::VaryingConfig *vc=shader_lib::GetVarying(ToUTF8String(item->text()));

    QString str;

    for(const shader_lib::Varying *v:*vc)
    {
        str.append(ToQString(v->type));
        str.append(" ");
        str.append(ToQString(v->name));
        str.append(";");

        if(!v->comment.IsEmpty())
        {
            str.append("    //");
            str.append(ToQString(v->comment));
        }

        str.append("\n");
    }

    glsl_editor->setPlainText(str);
}
