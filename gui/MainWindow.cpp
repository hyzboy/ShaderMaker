#include"MainWindow.h"
#include<QTabWidget>
#include"TP_ShaderLibrary.h"
#include"TP_MaterialLibrary.h"

void MainWindow::Init()
{
    this->setWindowTitle("MaterialBuilder v1.0 (Developing)");
    this->setWindowIcon(QIcon(":/res/logo256.png"));

    this->resize(QSize(960,540));

    InitMenu();

    QTabWidget *tab=new QTabWidget(this);

    {
        tab->addTab(new TPShaderLibrary(),"Shader Library");
        tab->addTab(new TPMaterialLibrary(),"Your Material Library");
    }

    this->setCentralWidget(tab);
}
