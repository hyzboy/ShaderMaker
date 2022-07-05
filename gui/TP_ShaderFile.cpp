#include"TP_ShaderFile.h"
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
#include<QVBoxLayout>
#include<QSplitter>
#include<QListWidget>
#include"W_ShaderEditor.h"
#include"WI_EditorTreeWidgetItem.h"

using namespace hgl;

TPShaderFile::TPShaderFile()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(Qt::Horizontal,this);
    
    layout->setContentsMargins(QMargins(0,0,0,0));
    
    //左侧文件树列表
    {
        file_tree_widget=new QTreeWidget(splitter);

        current_item=nullptr;

        UpdateFileTree();

        CreatePopupMenu();

        file_tree_widget->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(file_tree_widget,&QTreeWidget::itemPressed,this,&TPShaderFile::OnItemChange);
        connect(file_tree_widget,&QTreeWidget::itemDoubleClicked,this,&TPShaderFile::OnFileOpenEditor);
        connect(file_tree_widget,&QTreeWidget::customContextMenuRequested,this,&TPShaderFile::OnPopupMenu);
    }

    //右侧，编辑区
    {
        editor_tab_widget=new QTabWidget(splitter);
        editor_tab_widget->resize(QSize(width()*0.8,height()));
        editor_tab_widget->setTabsClosable(true);
        editor_tab_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

        connect(editor_tab_widget,&QTabWidget::tabCloseRequested,this,&TPShaderFile::OnTabCloseRequested);
    }
   
    layout->addWidget(splitter);
}

void TPShaderFile::OnItemChange(QTreeWidgetItem *item,int)
{
    EditorTreeWidgetItem *w=dynamic_cast<EditorTreeWidgetItem *>(item);

    current_item=w;
}

void TPShaderFile::OnFileOpenEditor(QTreeWidgetItem *item,int)
{
    EditorTreeWidgetItem *w=dynamic_cast<EditorTreeWidgetItem *>(item);

    if(w->isFolder())
        return;
    
    for(int i=0;i<editor_tab_widget->count();i++)
    {
        EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(i);

        if(w->GetItem()==item)
        {
            editor_tab_widget->setCurrentIndex(i);
            return;
        }
    }
    
    ShaderEditorWidget *widget=new ShaderEditorWidget(w);

    const int index=editor_tab_widget->addTab(widget,w->GetName());

    editor_tab_widget->setCurrentIndex(index);
}

void TPShaderFile::OnTabCloseRequested(int index)
{
    EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(index);

    if(!w)return;

    if(w->OnCloseRequested())
    {
        editor_tab_widget->removeTab(index);
    }
}