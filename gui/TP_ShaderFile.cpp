#include"TP_ShaderFile.h"
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<hgl/type/StringList.h>
#include<QVBoxLayout>
#include<QSplitter>
#include<QListWidget>

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

        connect(file_tree_widget,&QTreeWidget::itemDoubleClicked,this,&TPShaderFile::OnFileChanged);
    }

    //右侧，编辑区
    {
        glsl_editor=new GLSLTextEdit(splitter);
    }

    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,4);
    
    layout->addWidget(splitter);
}

void TPShaderFile::OnFileChanged(QTreeWidgetItem *item,int)
{
    glsl_editor->clear();

    const UTF8StringList *sl=shader_lib::GetRawModule(ToUTF8String(item->text((int)ShaderFileColumn::Name)));

    const UTF8String str=ToString(*sl,UTF8String("\n"));

    glsl_editor->setPlainText(ToQString(str));
}
