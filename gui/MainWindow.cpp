#include"MainWindow.h"
#include<QHBoxLayout>
#include<QTreeWidget>
#include<QTextBrowser>
#include<QSizePolicy>
#include<QSplitter>
#include<QListWidget>

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

   //���ShaderLib��/Material��
    {
        QTabWidget *left_tab=new QTabWidget(splitter);

        left_tab->addTab(CreateShaderLibraryWidget(),"Shader Library");
        left_tab->addTab(CreateMaterialLibraryWidget(),"Your Material Library");

        left_tab->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
    }

    //�Ҳ�shader�༭��
    {
        //QWidget *edit_area_widget=new QWidget(splitter);
        //QVBoxLayout *edit_area_layout=new QVBoxLayout(edit_area_widget);

        //�ڶ��ţ��༭��
        {
            QTabWidget *shader_tab=new QTabWidget(splitter);

            {
                shader_tab->addTab(CreateShaderEditorTabpage(),"Vertex");
                shader_tab->addTab(CreateShaderEditorTabpage(),"Geometry");
                shader_tab->addTab(CreateShaderEditorTabpage(),"Fragment");
            }
            shader_tab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
        }
    }

    {
        splitter->setStretchFactor(0,1);
        splitter->setStretchFactor(1,4);

        layout->addWidget(splitter);
    }

    this->setCentralWidget(widget);
}
