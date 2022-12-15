#include"TP_Module.h"
#include"ShaderLibrary/ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/type/MergeString.h>
#include<QVBoxLayout>
#include<QSplitter>
#include<QListWidget>
#include<QLabel>

using namespace hgl;

QSyntaxHighlighter *CreateGLSLSyntaxHighlighter(QTextDocument *document);

QListWidget *TPModule::CreateDependGroup(QLayout *depend_layout,const QString &name)
{
    QListWidget *list_widget;
    QWidget *group_widget=new QWidget(depend_layout->widget());
    QVBoxLayout *layout=new QVBoxLayout(group_widget);

    QMargins margins=layout->contentsMargins();
    margins.setLeft(0);
    margins.setRight(0);
    margins.setBottom(0);
    layout->setContentsMargins(margins);
                
    {
        QLabel *lab=new QLabel(group_widget);
        lab->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
        lab->setText(name);

        layout->addWidget(lab);

        list_widget=new QListWidget(group_widget);
        list_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

        layout->addWidget(list_widget);
    }

    depend_layout->addWidget(group_widget);

    return list_widget;
}

TPModule::TPModule()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Horizontal,this);
    
    layout->setContentsMargins(QMargins(0,0,0,0));

    //左侧，Varying列表
    {
        QListWidget *list=new QListWidget(splitter);
    
        list->setFrameShape(QFrame::StyledPanel);
        list->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

        for(const hgl::UTF8String *str:shader_lib::GetModuleList())
            list->addItem(ToQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPModule::OnModuleClicked);
    }

    //右侧，预览区
    {
        QWidget *right_area=new QWidget(splitter);
        QVBoxLayout *right_layout=new QVBoxLayout(right_area);
        QSplitter *right_splitter=new QSplitter(Qt::Vertical,right_area);

        right_layout->setContentsMargins(0,0,0,0);

        {
            QWidget *depend_area=new QWidget(right_splitter);
            QHBoxLayout *depend_layout=new QHBoxLayout(depend_area);

            depend_layout->widget();
            depend_layout->setContentsMargins(0,0,0,0);

            depend_raw_list=CreateDependGroup(depend_layout,"Shader Depend");
            depend_struct_list=CreateDependGroup(depend_layout,"Struct Depend");
        }

        {
            glsl_editor=new GLSLTextEdit(right_splitter);
        }

        right_splitter->setStretchFactor(0,1);
        right_splitter->setStretchFactor(1,4);

        right_layout->addWidget(right_splitter);
    }

    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,4);
    
    layout->addWidget(splitter);
}

TPModule::~TPModule()
{
}

void TPModule::OnModuleClicked(QListWidgetItem *item)
{
    glsl_editor->clear();
    
    shader_lib::XMLShaderModule *sm=shader_lib::GetShaderModule(ToUTF8String(item->text()));
    
    depend_raw_list->clear();
    for(const hgl::UTF8String *str:sm->depend_raw_list)
        depend_raw_list->addItem(ToQString(*str));

    depend_struct_list->clear();
    for(const hgl::UTF8String *str:sm->struct_list)
        depend_struct_list->addItem(ToQString(*str));

    const UTF8String str=MergeString(sm->codes,UTF8String("\n"));

    glsl_editor->setPlainText(ToQString(str));
}
