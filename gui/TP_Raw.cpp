#include"TP_Raw.h"
#include"ShaderLibrary/ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/type/MergeString.h>
#include<QVBoxLayout>
#include<QSplitter>
#include<QListWidget>

using namespace hgl;

TPRawGLSL::TPRawGLSL()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Horizontal,this);
    
    layout->setContentsMargins(QMargins(0,0,0,0));

    //左侧，Varying列表
    {
        QListWidget *list=new QListWidget(splitter);
    
        list->setFrameShape(QFrame::StyledPanel);
        list->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

        for(const hgl::UTF8String *str:shader_lib::GetRawModuleList())
            list->addItem(ToQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPRawGLSL::OnRawGLSLClicked);
    }

    //右侧，预览区
    {
        glsl_editor=new GLSLTextEdit(splitter);
    }

    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,4);
    
    layout->addWidget(splitter);
}

TPRawGLSL::~TPRawGLSL()
{
}

void TPRawGLSL::OnRawGLSLClicked(QListWidgetItem *item)
{
    glsl_editor->clear();

    const UTF8StringList *sl=shader_lib::GetRawModule(ToUTF8String(item->text()));

    const UTF8String str=MergeString(*sl,UTF8String("\n"));

    glsl_editor->setPlainText(ToQString(str));
}
