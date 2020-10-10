#include"MainWindow.h"
#include"DLG_About.h"
#include"DLG_PathConfig.h"
#include<QMenuBar>
#include<QApplication>

void MainWindow::InitMenu()
{
    QMenuBar *menu_bar=menuBar();

    {
        QMenu *file_menu=menu_bar->addMenu(tr("&File"));

        file_menu->addAction(tr("&New"),this,&MainWindow::NewMaterialFile);
        file_menu->addAction(tr("&Open"),this,&MainWindow::OpenMaterialFile);
    }

    {
        QMenu *setup_menu=menu_bar->addMenu(tr("&Setup"));

        setup_menu->addAction(tr("&Path"),this,&MainWindow::PathConfig);
    }
    
    {
        QMenu *about_menu=menu_bar->addMenu(tr("&About"));

        about_menu->addAction(tr("&About"),this,&MainWindow::about);
        about_menu->addAction(tr("About &Qt"),this,&QApplication::aboutQt);
    }
}

void MainWindow::about()
{
    DLGAbout *dlg=new DLGAbout();

    dlg->show();        
}

void MainWindow::NewMaterialFile()
{
}

void MainWindow::OpenMaterialFile()
{
}

void MainWindow::PathConfig()
{
    DLGPathConfig *dlg=new DLGPathConfig();
    
    dlg->show();
}