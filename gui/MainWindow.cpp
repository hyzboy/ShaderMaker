#include"MainWindow.h"
#include<QHBoxLayout>
#include<QTreeWidget>
#include<QTextBrowser>
#include<QSizePolicy>
#include<QSplitter>
#include<QListWidget>

#include"ShaderLib.h"
#include<hgl/type/QTString.h>

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
}

void MainWindow::ModuleClicked(QListWidgetItem *item)
{
}

QWidget *MainWindow::CreateShaderLibWidget(QWidget *parent)
{
    QWidget *widget=new QWidget(parent);
    QVBoxLayout *layout=new QVBoxLayout(widget);
    QSplitter *splitter=new QSplitter(Qt::Vertical,widget);

    {
        QTabWidget *tab=new QTabWidget(splitter);

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

QWidget *CreateShaderEditorTabpage()
{
    QTextBrowser *text=new QTextBrowser();
    text->setOpenExternalLinks(true);
    text->setFrameShape(QFrame::NoFrame);

    return text;
}

void MainWindow::Init()
{
    this->setWindowTitle("MaterialBuilder v1.0 (Developing)");
    this->setWindowIcon(QIcon(":/res/logo256.png"));

    this->resize(QSize(960,540));

    InitMenu();

    QWidget *widget=new QWidget(this);
    QHBoxLayout *layout=new QHBoxLayout(widget);
    QSplitter *splitter=new QSplitter(Qt::Horizontal,widget);

    {
        QWidget *shader_lib=CreateShaderLibWidget(splitter);

        shader_lib->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
    }

    {
        QTabWidget *shader_tab=new QTabWidget(splitter);

        {
            shader_tab->addTab(CreateShaderEditorTabpage(),"Vertex");
            shader_tab->addTab(CreateShaderEditorTabpage(),"Geometry");
            shader_tab->addTab(CreateShaderEditorTabpage(),"Fragment");
        }
        shader_tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    }

    {
        splitter->setStretchFactor(0,1);
        splitter->setStretchFactor(1,4);

        layout->addWidget(splitter);
    }

    this->setCentralWidget(widget);
}
