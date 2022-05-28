#include"MainWindow.h"
#include"DLG_About.h"
#include"DLG_Config.h"
#include<QMenuBar>
#include<QApplication>

void MainWindow::InitMenu()
{
    QMenuBar *menu_bar=menuBar();

    {
        //QMenu *setup_menu=menu_bar->addMenu(tr("&Setup"));

        menu_bar->addAction(tr("&Config"),this,&MainWindow::OnConfig);
    }
    
    {
        QMenu *about_menu=menu_bar->addMenu(tr("&About"));

        about_menu->addAction(tr("&About"),this,&MainWindow::OnAbout);
        about_menu->addAction(tr("About &Qt"),this,&QApplication::aboutQt);
    }
}

void MainWindow::OnAbout()
{
    DLGAbout *dlg=new DLGAbout();

    dlg->show();        
}

void MainWindow::OnConfig()
{
    DLGConfig *dlg=new DLGConfig();
    
    dlg->show();
}