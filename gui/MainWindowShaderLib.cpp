#include"MainWindow.h"
#include"ShaderLib.h"
#include<hgl/type/QTString.h>
#include<QSplitter>
#include<QVBoxLayout>

QSyntaxHighlighter *CreateGLSLSyntaxHighlighter(QTextDocument *document);

QListWidget *MainWindow::CreateListWidget(const UTF8StringList &str_list,ShaderLibItemClickedFunc func)
{
    QListWidget *list=new QListWidget;

    for(const hgl::UTF8String *str:str_list)
        list->addItem(toQString(*str));

    list->setFrameShape(QFrame::NoFrame);

    connect(list,&QListWidget::itemClicked,this,func);

    return list;
}

void MainWindow::VaryingClicked(QListWidgetItem *item)
{
    sl_preview->clear();

    const shader_lib::VaryingConfig *vc=shader_lib::GetVarying(toUTF8String(item->text()));

    QString str;

    for(const shader_lib::Varying *v:*vc)
    {
        str.append(toQString(v->type));
        str.append(" ");
        str.append(toQString(v->name));
        str.append(";");

        if(!v->comment.IsEmpty())
        {
            str.append("    //");
            str.append(toQString(v->comment));
        }

        str.append("\n");
    }

    sl_preview->setPlainText(str);
}

void MainWindow::RawClicked(QListWidgetItem *item)
{
    sl_preview->clear();

    const UTF8StringList *sl=shader_lib::GetRawModule(toUTF8String(item->text()));

    const UTF8String str=ToString(*sl,UTF8String("\n"));

    sl_preview->setPlainText(toQString(str));
}

void MainWindow::StructClicked(QListWidgetItem *item)
{
    sl_preview->clear();

    UTF8StringList sl;

    if(!shader_lib::AddStruct(sl,U8_TEXT("struct"),toUTF8String(item->text()),U8_TEXT("")))
        return;

    const UTF8String str=ToString(sl,UTF8String("\n"));

    sl_preview->setPlainText(toQString(str));
}

void MainWindow::ModuleClicked(QListWidgetItem *item)
{
    sl_preview->clear();

    shader_lib::XMLShaderModule *sm=shader_lib::GetShaderModule(toUTF8String(item->text()));
}

QWidget *MainWindow::CreateShaderLibraryWidget()
{
    QWidget *widget=new QWidget();
    QVBoxLayout *layout=new QVBoxLayout(widget);
    QSplitter *splitter=new QSplitter(Qt::Vertical,widget);

    layout->setContentsMargins(QMargins(0,0,0,0));

    {
        QTabWidget *tab=new QTabWidget(splitter);

        tab->setTabPosition(QTabWidget::TabPosition::South);

        {
            tab->addTab(CreateListWidget(shader_lib::GetVaryingList(),  &MainWindow::VaryingClicked ),"varying" );
            tab->addTab(CreateListWidget(shader_lib::GetRawModuleList(),&MainWindow::RawClicked     ),"raw"     );
            tab->addTab(CreateListWidget(shader_lib::GetStructList(),   &MainWindow::StructClicked  ),"struct"  );
            tab->addTab(CreateListWidget(shader_lib::GetModuleList(),   &MainWindow::ModuleClicked  ),"module"  );
        }

        tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    }

    {
        sl_preview=new QTextEdit(splitter);

        highlighter=CreateGLSLSyntaxHighlighter(sl_preview->document());

        sl_preview->setFrameShape(QFrame::NoFrame);
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

        sl_preview->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));   
    }

    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,1);

    layout->addWidget(splitter);

    return widget;
}