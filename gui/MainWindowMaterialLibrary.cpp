#include"MainWindow.h"
#include<QTreeWidget>

QWidget *MainWindow::CreateMaterialLibraryWidget()
{
    QTabWidget *tab=new QTabWidget();

    tab->setTabPosition(QTabWidget::TabPosition::South);

    {
        tab->addTab(new QTreeWidget(),"Vertex");
        tab->addTab(new QTreeWidget(),"Geometry");
        tab->addTab(new QTreeWidget(),"Fragment");
    }

    return tab;
}
