#include"TP_Struct.h"
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
#include<QVBoxLayout>
#include<QSplitter>
#include<QListWidget>

using namespace hgl;

QSyntaxHighlighter *CreateGLSLSyntaxHighlighter(QTextDocument *document);

TPStruct::TPStruct()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Horizontal,this);
    
    layout->setContentsMargins(QMargins(0,0,0,0));

    //左侧，Varying列表
    {
        QListWidget *list=new QListWidget(splitter);
    
        list->setFrameShape(QFrame::StyledPanel);
        list->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

        for(const hgl::UTF8String *str:shader_lib::GetStructList())
            list->addItem(ToQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPStruct::OnStructClicked);
    }

    //右侧，预览区
    {
        glsl_editor=new GLSLTextEdit(splitter);
    }

    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,4);
    
    layout->addWidget(splitter);
}

TPStruct::~TPStruct()
{
}

void TPStruct::OnStructClicked(QListWidgetItem *item)
{
    glsl_editor->clear();

    UTF8StringList sl;

    if(!shader_lib::AddStruct(sl,U8_TEXT("struct"),ToUTF8String(item->text()),U8_TEXT("")))
        return;

    const UTF8String str=ToString(sl,UTF8String("\n"));

    glsl_editor->setPlainText(ToQString(str));
}
