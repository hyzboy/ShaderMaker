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
            list->addItem(toQString(*str));

        connect(list,&QListWidget::itemClicked,this,&TPStruct::OnStructClicked);
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

TPStruct::~TPStruct()
{
}

void TPStruct::OnStructClicked(QListWidgetItem *item)
{
    sl_preview->clear();

    UTF8StringList sl;

    if(!shader_lib::AddStruct(sl,U8_TEXT("struct"),toUTF8String(item->text()),U8_TEXT("")))
        return;

    const UTF8String str=ToString(sl,UTF8String("\n"));

    sl_preview->setPlainText(toQString(str));
}
