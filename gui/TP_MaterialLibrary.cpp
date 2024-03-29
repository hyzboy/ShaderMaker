#include"TP_MaterialLibrary.h"
#include<QVBoxLayout>
#include<QSplitter>
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
#include"W_XMLShaderEditor.h"
#include"W_MaterialEditor.h"
#include"WI_EditorTreeWidgetItem.h"
#include"TypeDefine.h"

using namespace hgl;

TPMaterialLibrary::TPMaterialLibrary()
{
    QVBoxLayout *layout=new QVBoxLayout(this);
    QSplitter *splitter=new QSplitter(this);

    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(splitter);

    //左侧文件树列表
    {
        file_tree_widget=new QTreeWidget(splitter);

        current_item=nullptr;

        UpdateFileTree();

        //file_tree_widget->menu

        connect(file_tree_widget,&QTreeWidget::itemDoubleClicked,this,&TPMaterialLibrary::OnFileChanged);
    }

    //右侧编辑区
    {
        editor_tab_widget=new QTabWidget(splitter);
        editor_tab_widget->resize(QSize(width()*0.8,height()));
        editor_tab_widget->setTabsClosable(true);
        editor_tab_widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

        connect(editor_tab_widget,&QTabWidget::tabCloseRequested,this,&TPMaterialLibrary::OnTabCloseRequested);
    }

    layout->addWidget(splitter);
}

void TPMaterialLibrary::OnFileChanged(QTreeWidgetItem *item,int)
{
    if(current_item==item)return;
    
    EditorTreeWidgetItem *w=dynamic_cast<EditorTreeWidgetItem *>(item);

    if(w->isFolder())
        return;

    for(int i=0;i<editor_tab_widget->count();i++)
    {
        EditorWidget *ew=(EditorWidget *)editor_tab_widget->widget(i);

        if(ew->GetFilename()==w->GetFilename())
        {
            editor_tab_widget->setCurrentIndex(i);
            return;
        }
    }
    
    QWidget *widget=nullptr;

    if(w->type()==MaterialFileType::Material)
        widget=new MaterialEditorWidget(w->GetName(),w->GetFilename());
    else
        widget=new XMLShaderEditorWidget(w->GetName(),w->GetFilename());

    {
        const int index=editor_tab_widget->addTab(widget,QString(w->GetTypename())+QString(": ")+w->text(0));

        editor_tab_widget->setCurrentIndex(index);
    }
}

void TPMaterialLibrary::OnTabCloseRequested(int index)
{
    EditorWidget *w=(EditorWidget *)editor_tab_widget->widget(index);

    if(!w)return;

    if(w->OnCloseRequested())
    {
        editor_tab_widget->removeTab(index);
    }
}