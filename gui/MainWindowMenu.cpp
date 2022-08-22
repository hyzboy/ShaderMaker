#include"MainWindow.h"
#include"DLG_About.h"
#include"DLG_Config.h"
#include"DLG_GUIStyle.h"
#include"DLG_CreateMaterial.h"
#include<QApplication>
#include<QDesktopServices>
#include<hgl/qt/widgets/TipMenu.h>

void MainWindow::InitMenu()
{
    QMenuBar *menu_bar=menuBar();

    menu_bar->setStyle(QApplication::style());

    {
        QMenu *file_menu=menu_bar->addMenu(tr("&File"));

        file_menu->addAction(tr("&New Material"),this,&MainWindow::OnNewMaterial);
        file_menu->addSeparator();
        file_menu->addAction(tr("&Open Material"),this,&MainWindow::OnOpenMaterial);
        file_menu->addAction(tr("&Save Material"),this,&MainWindow::OnSaveMaterial);
        file_menu->addAction(tr("S&ave Material As"),this,&MainWindow::OnSaveMaterialAs);
        file_menu->addSeparator();
        file_menu->addAction(tr("&Close"),this,&MainWindow::OnClose);
        file_menu->addSeparator();
        file_menu->addAction(tr("&Exit"),this,&MainWindow::OnExit);
    }
    
    {
        QMenu *library_menu=new qt::TipMenu(tr("&Library"));

        menu_bar->addMenu(library_menu);

        {
            QAction *qa=library_menu->addAction(tr("&Varying"),this,&MainWindow::OnVaryingLibrary);        
            qa->setToolTip(tr("show a dialog that it sould show all varying."));
        }
        
        library_menu->addAction(tr("&Shader Module"),this,&MainWindow::OnShaderModuleLibrary);

        library_menu->addSeparator();
        library_menu->addAction(tr("&Reload"),this,&MainWindow::OnReloadLibrary);
    }

    {
        QMenu *setup_menu=menu_bar->addMenu(tr("&Setup"));

        setup_menu->addAction(tr("&Config"),this,&MainWindow::OnConfig);
        setup_menu->addSeparator();
        setup_menu->addAction(tr("Set UI &Style"),this,&MainWindow::OnStyle);
    }

    {
        QMenu *about_menu=menu_bar->addMenu(tr("&About"));

        about_menu->addAction(tr("&Homepage"),this,&MainWindow::OnHomepage);
        about_menu->addAction(tr("&About"),this,&MainWindow::OnAbout);
        about_menu->addAction(tr("About &Qt"),this,&QApplication::aboutQt);
    }
}

void MainWindow::OnHomepage()
{
    QDesktopServices::openUrl(QUrl(HGL_OFFICAL_WEB_URL_u8));
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

void MainWindow::OnStyle()
{
    DLGGUIStyle *dlg=new DLGGUIStyle();

    dlg->show();
}

void MainWindow::OnNewMaterial(){}
void MainWindow::OnOpenMaterial(){}
void MainWindow::OnSaveMaterial(){}
void MainWindow::OnSaveMaterialAs(){}
void MainWindow::OnClose(){}
void MainWindow::OnExit(){}

void MainWindow::OnVaryingLibrary()
{
    
}

void MainWindow::OnShaderModuleLibrary()
{
}

void MainWindow::OnReloadLibrary()
{
    
}
