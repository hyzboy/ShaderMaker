#include"MainWindow.h"
#include"DLG_About.h"
#include<QMenuBar>
#include<QApplication>

void MainWindow::Init()
{
    InitMenu();

    this->setWindowTitle("ShaderMaker v1.0");
    this->setWindowIcon(QIcon(":/res/logo256.png"));
}

void MainWindow::InitMenu()
{
    QMenuBar *menu_bar=menuBar();

    QMenu *about_menu=menu_bar->addMenu(tr("&About"));
    
    {
        about_menu->addAction(tr("&About"),this,&MainWindow::about);
        about_menu->addAction(tr("About &Qt"),this,&QApplication::aboutQt);
    }
}

void MainWindow::about()
{
    DLGAbout *dlg=new DLGAbout();

    dlg->show();        
}
