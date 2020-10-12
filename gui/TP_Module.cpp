#include"TP_Module.h"
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
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
            list->addItem(toQString(*str));

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
            sl_preview=new QTextEdit(right_splitter);

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
    sl_preview->clear();
    
    shader_lib::XMLShaderModule *sm=shader_lib::GetShaderModule(toUTF8String(item->text()));
    
    depend_raw_list->clear();
    for(const hgl::UTF8String *str:sm->depend_raw_list)
        depend_raw_list->addItem(toQString(*str));

    depend_struct_list->clear();
    for(const hgl::UTF8String *str:sm->struct_list)
        depend_struct_list->addItem(toQString(*str));

    const UTF8String str=ToString(sm->codes,UTF8String("\n"));

    sl_preview->setPlainText(toQString(str));
}
