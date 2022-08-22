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

    void OnNewMaterial();
    void OnOpenMaterial();
    void OnSaveMaterial();
    void OnSaveMaterialAs();
    void OnClose();
    void OnExit();

    void OnVaryingLibrary();
    void OnShaderModuleLibrary();
    void OnReloadLibrary();

    void OnHomepage();
    void OnAbout();

    void OnConfig();
    void OnStyle();

    void InitMenu();

public:

    using QMainWindow::QMainWindow;
    ~MainWindow()=default;

    void Init();
};//class MainWindow:public QMainWindow
