#include"MainWindow.h"
#include<QHBoxLayout>

/**
* +---------------------------------------------------------------------+
* | menubar                                                             |
* +---------------------------------------------------------------------+
* |+-------------+   +--------+ +----------+ +----------+               |
* || "ShaderLib" |   | Vertex | | Geometry | | Fragment |               |
* |+-------------+ +-+        +----------------------------------------+|
* || <TreeView>  | | in: position4f,Color4f,....                       ||
* || [ROOT]      | | out: position4f,Color4f,.....                     ||
* ||  +[varying] | | uniform: WorldMatrix world;                       ||
* ||  +[module]  | | raw: .........                                    ||
* ||  +[2dVector]| +---------------------------------------------------+|
* ||  +[...]     | | void main()                                       ||
* ||             | | {                                                 ||
* ||             | |  .....                                            ||
* ||             | | }                                                 ||
* |+-------------+ +---------------------------------------------------+|
* ++--------------------------------------------------------------------+
*/

void MainWindow::Init()
{
    InitMenu();

    this->setWindowTitle("MaterialEditor v1.0");
    this->setWindowIcon(QIcon(":/res/logo256.png"));

    QWidget *root_widget=new QWidget(this);
    QHBoxLayout *root_h_layout=new QHBoxLayout(root_widget);

    sl_tree=new QTreeWidget(this);
}