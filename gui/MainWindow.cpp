#include"MainWindow.h"

void MainWindow::Init()
{
    InitMenu();

    this->setWindowTitle("ShaderMaker v1.0");
    this->setWindowIcon(QIcon(":/res/logo256.png"));
}