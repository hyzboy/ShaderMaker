#include"TP_Varying.h"
#include"ShaderLibrary/ShaderLib.h"
#include"W_ShaderEditor.h"
#include"ConfigData.h"
#include<hgl/type/QTString.h>
#include<hgl/filesystem/Filename.h>
#include<QHBoxLayout>
#include<QListWidget>
#include<QScrollBar>

QSyntaxHighlighter *CreateGLSLSyntaxHighlighter(QTextDocument *document);

TPVarying::TPVarying()
{
    QHBoxLayout *layout=new QHBoxLayout(this);
    
    layout->setContentsMargins(QMargins(0,0,0,0));

    //左侧，Varying列表
    {
        QListWidget *list=new QListWidget(this);
    
        list->setFrameShape(QFrame::StyledPanel);

        for(const hgl::UTF8String *str:shader_lib::GetVaryingList())
            list->addItem(ToQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPVarying::OnVaryingClicked);

        list->setFixedWidth(list->sizeHintForColumn(0)+
                            list->verticalScrollBar()->width());

        layout->addWidget(list);
    }

    //右侧，编辑区
    {
        editor_tab_widget=new QTabWidget(this);
        editor_tab_widget->resize(QSize(width()*0.8,height()));
        editor_tab_widget->setTabsClosable(true);

        connect(editor_tab_widget,&QTabWidget::tabCloseRequested,this,&TPVarying::OnTabCloseRequested);        
    }
   
    layout->addWidget(editor_tab_widget);
}

void TPVarying::OnVaryingClicked(QListWidgetItem *item)
{
    const QString &name=item->text();
    
    for(int i=0;i<editor_tab_widget->count();i++)
    {
        EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(i);

        if(w->GetItemName()==name)
        {
            editor_tab_widget->setCurrentIndex(i);
            return;
        }
    }
    
    const OSString &varying_path=GetVaryingLibraryPath();

    const OSString &filename=filesystem::MergeFilename(varying_path,ToOSString(name))+OS_TEXT(".varying");
    
    ShaderEditorWidget *widget=new ShaderEditorWidget(name,filename,U8_TEXT("Varying"));

    const int index=editor_tab_widget->addTab(widget,name);

    editor_tab_widget->setCurrentIndex(index);
}

void TPVarying::OnTabCloseRequested(int index)
{
    EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(index);

    if(!w)return;

    if(w->OnCloseRequested())
    {
        editor_tab_widget->removeTab(index);
    }
}