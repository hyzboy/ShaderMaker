#include"MainWindow.h"
#include"DLG_About.h"
#include"DLG_Config.h"
#include"DLG_CreateMaterial.h"
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
        QMenu *create_menu=menu_bar->addMenu(tr("Create"));

        {
            QMenu *create_shader_menu=create_menu->addMenu(tr("Create Shader"));
            
            create_shader_menu->addAction(tr("Create Vertex Shader"),this,&MainWindow::OnCreateVertexShader);
            create_shader_menu->addAction(tr("Create Tessellation Control Shader"),this,&MainWindow::OnCreateTessControlShader);
            create_shader_menu->addAction(tr("Create Tessellation Evaluation Shader"),this,&MainWindow::OnCreateTessEvalShader);
            create_shader_menu->addAction(tr("Create Gemoetry Shader"),this,&MainWindow::OnCreateGeometryShader);
            create_shader_menu->addAction(tr("Create Fragment Shader"),this,&MainWindow::OnCreateFragmentShader);
        }
        
        create_menu->addAction(tr("Create &Material"),this,&MainWindow::OnCreateMaterial);
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

void MainWindow::OnCreateVertexShader()
{}
void MainWindow::OnCreateTessControlShader()
{}
void MainWindow::OnCreateTessEvalShader()
{}
void MainWindow::OnCreateGeometryShader()
{}
void MainWindow::OnCreateFragmentShader()
{}

void MainWindow::OnCreateMaterial()
{
    DLGCreateMaterial *dlg=new DLGCreateMaterial();

    dlg->show();
}