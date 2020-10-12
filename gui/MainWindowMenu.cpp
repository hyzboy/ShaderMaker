#include"MainWindow.h"
#include"DLG_About.h"
#include"DLG_PathConfig.h"
#include<QMenuBar>
#include<QApplication>

void MainWindow::InitMenu()
{
    QMenuBar *menu_bar=menuBar();

    {
        QMenu *setup_menu=menu_bar->addMenu(tr("&Setup"));

        setup_menu->addAction(tr("&Path"),this,&MainWindow::OnPathConfig);
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

void MainWindow::OnPathConfig()
{
    DLGPathConfig *dlg=new DLGPathConfig();
    
    dlg->show();
}