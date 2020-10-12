#include<QMainWindow>
#include<QTreeWidgetItem>
#include<QListWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<hgl/type/StringList.h>

using namespace hgl;

class MainWindow:public QMainWindow
{
    Q_OBJECT

private:    //menu

    void OnAbout();

    void OnNewMaterialFile();
    void OnOpenMaterialFile();
    void OnPathConfig();

    void InitMenu();

public:

    using QMainWindow::QMainWindow;
    ~MainWindow()=default;

    void Init();
};//class MainWindow:public QMainWindow
